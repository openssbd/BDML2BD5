#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <list>
#include <memory>
#include <sstream>
#include <unistd.h>

#include "bdml0.18.hxx"
#include "xyzt.h"
#include "object.h"
#include "feature.h"
#include "sunit.h"
#include "series.h"
#include "trackinfo.h"
#include "property.h"
#include "pelement.h"

#include "hdf5.h"
#include "hdf5_hl.h"
#include "H5Cpp.h"

using namespace std;

#define LENGTH8 8
#define LENGTH16 16
#define LENGTH32 32
#define LENGTH64 64
#define LENGTH128 128
#define LENGTH256 256
#define FILENAME "bd5.bd5.h5"
#define ODNAME "objectDef"
#define FDNAME "featureDef"
#define SUNAME "scaleUnit"
#define TINAME "trackInfo"
const std::string member_number("Number");
const std::string member_name("Name");

// objectDef definition
typedef struct {
  int oID;
  char name[LENGTH128];
} objectDef;

// featureDef definition
typedef struct {
  int fID;
  char name[LENGTH128];
  char fUnit[LENGTH16];
} featureDef;

// scaleUnit definition
typedef struct {
  char dimension[LENGTH8];
  double xScale;
  double yScale;
  double zScale;
  char sUnit[LENGTH16];
  double tScale;
  char tUnit[LENGTH16];
} scaleUnit3DT;

typedef struct {
  char dimension[LENGTH8];
  double xScale;
  double yScale;
  char sUnit[LENGTH16];
  double tScale;
  char tUnit[LENGTH16];
} scaleUnit2DT;

typedef struct {
  char dimension[LENGTH8];
  double xScale;
  char sUnit[LENGTH16];
  double tScale;
  char tUnit[LENGTH16];
} scaleUnit1DT;

typedef struct {
  char dimension[LENGTH8];
  double tScale;
  char tUnit[LENGTH16];
} scaleUnit0DT;

typedef struct {
  char dimension[LENGTH8];
  double xScale;
  double yScale;
  double zScale;
  char sUnit[LENGTH16];
} scaleUnit3D;

typedef struct {
  char dimension[LENGTH8];
  double xScale;
  double yScale;
  char sUnit[LENGTH16];
} scaleUnit2D;

typedef struct {
  char dimension[LENGTH8];
  double xScale;
  char sUnit[LENGTH16];
} scaleUnit1D;

typedef struct { char dimension[LENGTH8]; } scaleUnit0D;

// object definition
typedef struct {
  char ID[LENGTH8];
  double t;
  char entity[LENGTH8];
  int seqID;
  double x;
  double y;
  double z;
  double r;
  char label[LENGTH8];
} object5;

// object definition (length16)
typedef struct {
  char ID[LENGTH16];
  double t;
  char entity[LENGTH8];
  int seqID;
  double x;
  double y;
  double z;
  double r;
  char label[LENGTH16];
} object16;

// feature definition
typedef struct {
  char ID[LENGTH8];
  int fID;
  double value;
} feature5;

// feature definition (length16)
typedef struct {
  char ID[LENGTH16];
  int fID;
  double value;
} feature16;

// trackinfo definition
typedef struct {
  char from[LENGTH8];
  char to[LENGTH8];
} trackinfo5;

// trackinfo definition (length16)
typedef struct {
  char from[LENGTH16];
  char to[LENGTH16];
} trackinfo16;

int main(int argc, char *argv[]) {
  try {
    if (argc < 2) {
      cerr << "usage: " << argv[0] << " *.bdml0.18.xml" << endl;
      return 1;
    }

    string filename = argv[1];
    int hh = filename.find(".bdml0.18.xml");
    filename.erase(hh);
    cerr << filename << endl;
    filename = filename + ".bd5.h5";

    hid_t file_id, data_id, su_id, od_id, fd_id, odir_id, sdir_id, fdir_id,
        so_id, sf_id, si_id;
    hid_t so_sid, sf_sid, su_sid, od_sid, si_sid, fd_sid;
    hid_t so_tid, sf_tid, su_tid, od_tid, si_tid, fd_tid;
    herr_t status;
    file_id =
        H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    //    file_id = H5Fcreate(FILENAME, H5F_ACC_TRUNC, H5P_DEFAULT,
    //    H5P_DEFAULT);
    data_id =
        H5Gcreate2(file_id, "/data", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    int rank;

    const H5::StrType strname8(H5::PredType::C_S1, LENGTH8);
    const H5::StrType strname16(H5::PredType::C_S1, LENGTH16);
    const H5::StrType strname32(H5::PredType::C_S1, LENGTH32);
    const H5::StrType strname64(H5::PredType::C_S1, LENGTH64);
    const H5::StrType strname128(H5::PredType::C_S1, LENGTH128);
    const H5::StrType strname256(H5::PredType::C_S1, LENGTH256);

    auto_ptr<bdml::bdmlDocument> h(bdml::bdml(argv[1]));

    for (int i = 2; i < argc; i++) {
      auto_ptr<bdml::bdmlDocument> r(bdml::bdml(argv[i]));
      if (r->data()) {
        for (bdml::Data::component_const_iterator p =
                 r->data()->component().begin();
             p != r->data()->component().end(); p++) {
          h->data()->component().push_back(*p);
        }
      }
    }

    double xscale = 0.0;
    xscale = h->data()->scaleUnit().xScale();
    double yscale = 0.0;
    yscale = h->data()->scaleUnit().yScale();
    double zscale = 0.0;
    zscale = h->data()->scaleUnit().zScale();
    string xyzunit = "";
    xyzunit = h->data()->scaleUnit().xyzUnit();
    double tscale = 0.0;
    tscale = h->data()->scaleUnit().tScale();
    string tunit = "";
    tunit = h->data()->scaleUnit().tUnit();

    // write scaleUnit
    hsize_t dim[1];

    int tmp_dim = 0;
    if (tscale != 0.0) {
      if (xscale != 0.0 && yscale != 0.0 && zscale != 0.0) {
        scaleUnit3DT sunit[1];
        strcpy(sunit[0].dimension, "3D+T");
        sunit[0].xScale = xscale;
        sunit[0].yScale = yscale;
        sunit[0].zScale = zscale;
        strcpy(sunit[0].sUnit, xyzunit.c_str());
        sunit[0].tScale = tscale;
        strcpy(sunit[0].tUnit, tunit.c_str());

        dim[0] = sizeof(sunit) / sizeof(scaleUnit3DT);
        rank = sizeof(dim) / sizeof(hsize_t);

        su_tid = H5Tcreate(H5T_COMPOUND, sizeof(scaleUnit3DT));
        H5Tinsert(su_tid, "dimension", HOFFSET(scaleUnit3DT, dimension),
                  strname8.getId());
        H5Tinsert(su_tid, "xScale", HOFFSET(scaleUnit3DT, xScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "yScale", HOFFSET(scaleUnit3DT, yScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "zScale", HOFFSET(scaleUnit3DT, zScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "sUnit", HOFFSET(scaleUnit3DT, sUnit),
                  strname16.getId());
        H5Tinsert(su_tid, "tScale", HOFFSET(scaleUnit3DT, tScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "tUnit", HOFFSET(scaleUnit3DT, tUnit),
                  strname16.getId());
        su_sid = H5Screate_simple(rank, dim, NULL);
        su_id = H5Dcreate1(data_id, SUNAME, su_tid, su_sid, H5P_DEFAULT);
        status = H5Dwrite(su_id, su_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sunit);
        H5Dclose(su_id);
        H5Sclose(su_sid);
        H5Tclose(su_tid);

        tmp_dim = 3;
      } else if (xscale != 0.0 && yscale != 0.0 && zscale == 0.0) {
        scaleUnit2DT sunit[1];
        strcpy(sunit[0].dimension, "2D+T");
        sunit[0].xScale = xscale;
        sunit[0].yScale = yscale;
        strcpy(sunit[0].sUnit, xyzunit.c_str());
        sunit[0].tScale = tscale;
        strcpy(sunit[0].tUnit, tunit.c_str());

        dim[0] = sizeof(sunit) / sizeof(scaleUnit2DT);
        rank = sizeof(dim) / sizeof(hsize_t);

        su_tid = H5Tcreate(H5T_COMPOUND, sizeof(scaleUnit2DT));
        H5Tinsert(su_tid, "dimension", HOFFSET(scaleUnit2DT, dimension),
                  strname8.getId());
        H5Tinsert(su_tid, "xScale", HOFFSET(scaleUnit2DT, xScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "yScale", HOFFSET(scaleUnit2DT, yScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "sUnit", HOFFSET(scaleUnit2DT, sUnit),
                  strname16.getId());
        H5Tinsert(su_tid, "tScale", HOFFSET(scaleUnit2DT, tScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "tUnit", HOFFSET(scaleUnit2DT, tUnit),
                  strname16.getId());
        su_sid = H5Screate_simple(rank, dim, NULL);
        su_id = H5Dcreate1(data_id, SUNAME, su_tid, su_sid, H5P_DEFAULT);
        status = H5Dwrite(su_id, su_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sunit);
        H5Dclose(su_id);
        H5Sclose(su_sid);
        H5Tclose(su_tid);

        tmp_dim = 2;
      } else if (xscale != 0.0 && yscale == 0.0 && zscale == 0.0) {
        scaleUnit1DT sunit[1];
        strcpy(sunit[0].dimension, "1D+T");
        sunit[0].xScale = xscale;
        strcpy(sunit[0].sUnit, xyzunit.c_str());
        sunit[0].tScale = tscale;
        strcpy(sunit[0].tUnit, tunit.c_str());

        dim[0] = sizeof(sunit) / sizeof(scaleUnit1DT);
        rank = sizeof(dim) / sizeof(hsize_t);

        su_tid = H5Tcreate(H5T_COMPOUND, sizeof(scaleUnit1DT));
        H5Tinsert(su_tid, "dimension", HOFFSET(scaleUnit1DT, dimension),
                  strname8.getId());
        H5Tinsert(su_tid, "xScale", HOFFSET(scaleUnit1DT, xScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "sUnit", HOFFSET(scaleUnit1DT, sUnit),
                  strname16.getId());
        H5Tinsert(su_tid, "tScale", HOFFSET(scaleUnit1DT, tScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "tUnit", HOFFSET(scaleUnit1DT, tUnit),
                  strname16.getId());
        su_sid = H5Screate_simple(rank, dim, NULL);
        su_id = H5Dcreate1(data_id, SUNAME, su_tid, su_sid, H5P_DEFAULT);
        status = H5Dwrite(su_id, su_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sunit);
        H5Dclose(su_id);
        H5Sclose(su_sid);
        H5Tclose(su_tid);

        tmp_dim = 1;
      } else if (xscale == 0.0 && yscale == 0.0 && zscale == 0.0) {
        scaleUnit0DT sunit[1];
        strcpy(sunit[0].dimension, "0D+T");
        sunit[0].tScale = tscale;
        strcpy(sunit[0].tUnit, tunit.c_str());

        dim[0] = sizeof(sunit) / sizeof(scaleUnit0DT);
        rank = sizeof(dim) / sizeof(hsize_t);

        su_tid = H5Tcreate(H5T_COMPOUND, sizeof(scaleUnit0DT));
        H5Tinsert(su_tid, "dimension", HOFFSET(scaleUnit0DT, dimension),
                  strname8.getId());
        H5Tinsert(su_tid, "tScale", HOFFSET(scaleUnit0DT, tScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "tUnit", HOFFSET(scaleUnit0DT, tUnit),
                  strname16.getId());
        su_sid = H5Screate_simple(rank, dim, NULL);
        su_id = H5Dcreate1(data_id, SUNAME, su_tid, su_sid, H5P_DEFAULT);
        status = H5Dwrite(su_id, su_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sunit);
        H5Dclose(su_id);
        H5Sclose(su_sid);
        H5Tclose(su_tid);

        tmp_dim = 0;
      } else {
        cerr << "xscale = " << xscale << ", yscale = " << yscale
             << ", zscale = " << zscale << endl;
        exit(-1);
      }
    } else if (tscale == 0.0) {
      if (xscale != 0.0 && yscale != 0.0 && zscale != 0.0) {
        scaleUnit3D sunit[1];
        strcpy(sunit[0].dimension, "3D");
        sunit[0].xScale = xscale;
        sunit[0].yScale = yscale;
        sunit[0].zScale = zscale;
        strcpy(sunit[0].sUnit, xyzunit.c_str());

        dim[0] = sizeof(sunit) / sizeof(scaleUnit3D);
        rank = sizeof(dim) / sizeof(hsize_t);

        su_tid = H5Tcreate(H5T_COMPOUND, sizeof(scaleUnit3D));
        H5Tinsert(su_tid, "dimension", HOFFSET(scaleUnit3D, dimension),
                  strname8.getId());
        H5Tinsert(su_tid, "xScale", HOFFSET(scaleUnit3D, xScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "yScale", HOFFSET(scaleUnit3D, yScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "zScale", HOFFSET(scaleUnit3D, zScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "sUnit", HOFFSET(scaleUnit3D, sUnit),
                  strname16.getId());
        su_sid = H5Screate_simple(rank, dim, NULL);
        su_id = H5Dcreate1(data_id, SUNAME, su_tid, su_sid, H5P_DEFAULT);
        status = H5Dwrite(su_id, su_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sunit);
        H5Dclose(su_id);
        H5Sclose(su_sid);
        H5Tclose(su_tid);

        tmp_dim = 3;
      } else if (xscale != 0.0 && yscale != 0.0 && zscale == 0.0) {
        scaleUnit2D sunit[1];
        strcpy(sunit[0].dimension, "2D");
        sunit[0].xScale = xscale;
        sunit[0].yScale = yscale;
        strcpy(sunit[0].sUnit, xyzunit.c_str());

        dim[0] = sizeof(sunit) / sizeof(scaleUnit2D);
        rank = sizeof(dim) / sizeof(hsize_t);

        su_tid = H5Tcreate(H5T_COMPOUND, sizeof(scaleUnit2D));
        H5Tinsert(su_tid, "dimension", HOFFSET(scaleUnit2D, dimension),
                  strname8.getId());
        H5Tinsert(su_tid, "xScale", HOFFSET(scaleUnit2D, xScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "yScale", HOFFSET(scaleUnit2D, yScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "sUnit", HOFFSET(scaleUnit2D, sUnit),
                  strname16.getId());
        su_sid = H5Screate_simple(rank, dim, NULL);
        su_id = H5Dcreate1(data_id, SUNAME, su_tid, su_sid, H5P_DEFAULT);
        status = H5Dwrite(su_id, su_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sunit);
        H5Dclose(su_id);
        H5Sclose(su_sid);
        H5Tclose(su_tid);

        tmp_dim = 2;
      } else if (xscale != 0.0 && yscale == 0.0 && zscale == 0.0) {
        scaleUnit1D sunit[1];
        strcpy(sunit[0].dimension, "1D");
        sunit[0].xScale = xscale;
        strcpy(sunit[0].sUnit, xyzunit.c_str());

        dim[0] = sizeof(sunit) / sizeof(scaleUnit1D);
        rank = sizeof(dim) / sizeof(hsize_t);

        su_tid = H5Tcreate(H5T_COMPOUND, sizeof(scaleUnit1D));
        H5Tinsert(su_tid, "dimension", HOFFSET(scaleUnit1D, dimension),
                  strname8.getId());
        H5Tinsert(su_tid, "xScale", HOFFSET(scaleUnit1D, xScale),
                  H5T_NATIVE_DOUBLE);
        H5Tinsert(su_tid, "sUnit", HOFFSET(scaleUnit1D, sUnit),
                  strname16.getId());
        su_sid = H5Screate_simple(rank, dim, NULL);
        su_id = H5Dcreate1(data_id, SUNAME, su_tid, su_sid, H5P_DEFAULT);
        status = H5Dwrite(su_id, su_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sunit);
        H5Dclose(su_id);
        H5Sclose(su_sid);
        H5Tclose(su_tid);

        tmp_dim = 1;
      } else if (xscale == 0.0 && yscale == 0.0 && zscale == 0.0) {
        scaleUnit0D sunit[1];
        strcpy(sunit[0].dimension, "0D");

        dim[0] = sizeof(sunit) / sizeof(scaleUnit0D);
        rank = sizeof(dim) / sizeof(hsize_t);

        su_tid = H5Tcreate(H5T_COMPOUND, sizeof(scaleUnit0D));
        H5Tinsert(su_tid, "dimension", HOFFSET(scaleUnit0D, dimension),
                  strname8.getId());
        su_sid = H5Screate_simple(rank, dim, NULL);
        su_id = H5Dcreate1(data_id, SUNAME, su_tid, su_sid, H5P_DEFAULT);
        status = H5Dwrite(su_id, su_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, sunit);
        H5Dclose(su_id);
        H5Sclose(su_sid);
        H5Tclose(su_tid);

        tmp_dim = 0;
      } else {
        cerr << "xscale = " << xscale << ", yscale = " << yscale
             << ", zscale = " << zscale << endl;
        exit(-1);
      }
    } else {
      cerr << "tscale = " << tscale << endl;
      exit(-1);
    }

    // object list
    list<object> objlist;
    objlist.clear();
    list<object>::iterator oit;

    int count = 0;
    for (bdml::Data::object_const_iterator p = h->data()->object().begin();
         p != h->data()->object().end(); p++) {
      object tmp_obj(count, p->objectName());
      objlist.push_back(tmp_obj);
      count++;
    }

    // write object_list
    count = 0;
    objectDef objectlist[objlist.size()];
    for (oit = objlist.begin(); oit != objlist.end(); oit++) {
      objectlist[count].oID = count;
      strcpy(objectlist[count].name, oit->getS().c_str());
      count++;
    }

    dim[0] = sizeof(objectlist) / sizeof(objectDef);
    rank = sizeof(dim) / sizeof(hsize_t);

    od_tid = H5Tcreate(H5T_COMPOUND, sizeof(objectDef));
    H5Tinsert(od_tid, "oID", HOFFSET(objectDef, oID), H5T_NATIVE_INT);
    H5Tinsert(od_tid, "name", HOFFSET(objectDef, name), strname128.getId());
    od_sid = H5Screate_simple(rank, dim, NULL);
    od_id = H5Dcreate1(data_id, ODNAME, od_tid, od_sid, H5P_DEFAULT);
    status = H5Dwrite(od_id, od_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objectlist);
    H5Dclose(od_id);
    H5Sclose(od_sid);
    H5Tclose(od_tid);

    int fnn = h->data()->feature().size();
    cerr << fnn << endl;

    // feature list
    list<feature> featlist;
    featlist.clear();
    list<feature>::iterator fit;

    if (fnn != 0) {

      count = 0;
      for (bdml::Data::feature_const_iterator p = h->data()->feature().begin();
           p != h->data()->feature().end(); p++) {
        feature tmp_feat(count, p->featureName(), p->featureUnit());
        featlist.push_back(tmp_feat);
        count++;
      }

      // write feature_list
      count = 0;
      featureDef featurelist[featlist.size()];
      for (fit = featlist.begin(); fit != featlist.end(); fit++) {
        featurelist[count].fID = count;
        strcpy(featurelist[count].name, fit->getS().c_str());
        strcpy(featurelist[count].fUnit, fit->getU().c_str());
        count++;
      }

      dim[0] = sizeof(featurelist) / sizeof(featureDef);
      rank = sizeof(dim) / sizeof(hsize_t);

      fd_tid = H5Tcreate(H5T_COMPOUND, sizeof(featureDef));
      H5Tinsert(fd_tid, "fID", HOFFSET(featureDef, fID), H5T_NATIVE_INT);
      H5Tinsert(fd_tid, "name", HOFFSET(featureDef, name), strname128.getId());
      H5Tinsert(fd_tid, "fUnit", HOFFSET(featureDef, fUnit), strname16.getId());
      fd_sid = H5Screate_simple(rank, dim, NULL);
      fd_id = H5Dcreate1(data_id, "featureDef", fd_tid, fd_sid, H5P_DEFAULT);
      status =
          H5Dwrite(fd_id, fd_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, featurelist);
      H5Dclose(fd_id);
      H5Sclose(fd_sid);
      H5Tclose(fd_tid);
    }

    // create objects dir
    //    odir_id = H5Gcreate2(fild_id, "/data/objects", H5P_DEFAULT,
    //    H5P_DEFAULT, H5P_DEFAULT);

    // create features dir
    //    fdir_id = H5Gcreate2(file_id, "/data/features", H5P_DEFAULT,
    //    H5P_DEFAULT, H5P_DEFAULT);

    list<double> tlist;
    tlist.clear();
    list<double>::iterator tt;

    for (bdml::Data::component_const_iterator p =
             h->data()->component().begin();
         p != h->data()->component().end(); p++) {
      tlist.push_back(p->time());
    }

    tlist.sort();
    tlist.unique();
    int tpsize = tlist.size();

    trackinfo ti;

    series ser[tpsize];
    int tc = 0;
    for (tt = tlist.begin(); tt != tlist.end(); tt++) {
      ser[tc].setT(*tt);
      //      cerr << ser[tc].getOE().size() << endl;
      tc++;
    }

    for (bdml::Data::component_const_iterator p =
             h->data()->component().begin();
         p != h->data()->component().end(); p++) {
      double t = p->time();
      string tmp_id = p->componentID();
      string tmp_label = "";
      if (p->componentName()) {
        tmp_label = tmp_label.append(p->componentName()->c_str());
      }
      for (bdml::Component::prevID_const_iterator r = p->prevID().begin();
           r != p->prevID().end(); r++) {
        ti.addFromTo(*r, tmp_id);
      }
      for (bdml::Component::measurement_const_iterator r =
               p->measurement().begin();
           r != p->measurement().end(); r++) {
        string obj_ref = r->objectRef();
        int objnum = 0;
        for (oit = objlist.begin(); oit != objlist.end(); oit++) {
          if (!strcmp(obj_ref.c_str(), oit->getS().c_str())) {
            break;
          }
          objnum++;
        }
        if (r->point()) {
          for (bdml::Point::xyz_const_iterator q = r->point()->xyz().begin();
               q != r->point()->xyz().end(); q++) {
            double x, y, z;
            x = q->x();
            y = q->y();
            z = q->z();
            xyzt tmp_xyzt(tmp_id, x, y, z, 0.0, t, 0, tmp_label, "point");
            for (int i = 0; i < tpsize; i++) {
              if (ser[i].getT() == t) {
                ser[i].addPoint(objnum, tmp_xyzt);
                break;
              }
            }
          }
        } else if (r->line()) {
          int seqnum = 0;
          for (bdml::Line::xyzSequence_const_iterator q =
                   r->line()->xyzSequence().begin();
               q != r->line()->xyzSequence().end(); q++) {
            for (bdml::XYZSequence::xyz_const_iterator s = q->xyz().begin();
                 s != q->xyz().end(); s++) {
              double x, y, z;
              x = s->x();
              y = s->y();
              z = s->z();
              xyzt tmp_xyzt(tmp_id, x, y, z, 0.0, t, seqnum, tmp_label, "line");
              for (int i = 0; i < tpsize; i++) {
                if (ser[i].getT() == t) {
                  ser[i].addLine(objnum, tmp_xyzt);
                  break;
                }
              }
            }
            seqnum++;
          }
        } else if (r->circle()) {
          double x, y, z, rad;
          x = r->circle()->xyz().x();
          y = r->circle()->xyz().y();
          z = r->circle()->xyz().z();
          rad = r->circle()->radius();
          xyzt tmp_xyzt(tmp_id, x, y, z, rad, t, 0, tmp_label, "circle");
          for (int i = 0; i < tpsize; i++) {
            if (ser[i].getT() == t) {
              ser[i].addCircle(objnum, tmp_xyzt);
              break;
            }
          }
        } else if (r->sphere()) {
          double x, y, z, rad;
          x = r->sphere()->xyz().x();
          y = r->sphere()->xyz().y();
          z = r->sphere()->xyz().z();
          rad = r->sphere()->radius();
          xyzt tmp_xyzt(tmp_id, x, y, z, rad, t, 0, tmp_label, "sphere");
          for (int i = 0; i < tpsize; i++) {
            if (ser[i].getT() == t) {
              ser[i].addSphere(objnum, tmp_xyzt);
              break;
            }
          }
        } else if (r->face()) {
          int seqnum = 0;
          for (bdml::Face::xyzTriplet_const_iterator q =
                   r->face()->xyzTriplet().begin();
               q != r->face()->xyzTriplet().end(); q++) {
            for (bdml::XYZTriplet::xyz_const_iterator s = q->xyz().begin();
                 s != q->xyz().end(); s++) {
              double x, y, z;
              x = s->x();
              y = s->y();
              z = s->z();
              xyzt tmp_xyzt(tmp_id, x, y, z, 0.0, t, seqnum, tmp_label, "face");
              for (int i = 0; i < tpsize; i++) {
                if (ser[i].getT() == t) {
                  ser[i].addFace(objnum, tmp_xyzt);
                  break;
                }
              }
            }
            seqnum++;
          }

        } else {
          xyzt tmp_xyzt(tmp_id, 0.0, 0.0, 0.0, 0.0, t, 0, tmp_label, "object");
          for (int i = 0; i < tpsize; i++) {
            if (ser[i].getT() == t) {
              ser[i].addObject(objnum, tmp_xyzt);
              break;
            }
          }
        }

        for (bdml::Measurement::property_const_iterator u =
                 r->property().begin();
             u != r->property().end(); u++) {
          string propname;
          propname = u->featureRef();
          double propval = 0.0;
          propval = u->featureVal();
          int featnum = 0;
          for (fit = featlist.begin(); fit != featlist.end(); fit++) {
            if (!strcmp(propname.c_str(), fit->getS().c_str())) {
              break;
            }
            featnum++;
          }

          pelement tmp_pe(tmp_id, featnum, propval);
          for (int i = 0; i < tpsize; i++) {
            if (ser[i].getT() == t) {
              ser[i].addProperty(objnum, tmp_pe);
              break;
            }
          }
        }
      }
    }

    cerr << "finish to set data" << endl;

    list<objectentity>::iterator e;
    list<property>::iterator pp;
    list<xyzt>::iterator ss;
    list<pelement>::iterator sss;

    // determine the length
    unsigned int max_len = 0;
    for (int i = 0; i < tpsize; i++) {
      //      cerr << i << endl;
      list<objectentity> tmp_oe = ser[i].getOE();
      for (e = tmp_oe.begin(); e != tmp_oe.end(); e++) {
        //        cerr << e->getS().c_str() << endl;
        if (!strcmp(e->getS().c_str(), "point")) {
          list<xyzt> tmp_point;
          tmp_point = e->getPoint();

          for (ss = tmp_point.begin(); ss != tmp_point.end(); ss++) {
            if (ss->getId().size() > max_len)
              max_len = ss->getId().size();
            if (ss->getL().size() > max_len)
              max_len = ss->getL().size();
          }
        } else if (!strcmp(e->getS().c_str(), "line")) {
          list<xyzt> tmp_line;
          tmp_line = e->getLine();

          for (ss = tmp_line.begin(); ss != tmp_line.end(); ss++) {
            //            cerr << "hello" << endl;
            //            cerr << ss->getId().size() << endl;
            if (ss->getId().size() > max_len)
              max_len = ss->getId().size();
            if (ss->getL().size() > max_len)
              max_len = ss->getL().size();
          }
        } else if (!strcmp(e->getS().c_str(), "circle")) {
          list<xyzt> tmp_circle;
          tmp_circle = e->getCircle();

          for (ss = tmp_circle.begin(); ss != tmp_circle.end(); ss++) {
            if (ss->getId().size() > max_len)
              max_len = ss->getId().size();
            if (ss->getL().size() > max_len)
              max_len = ss->getL().size();
          }
        } else if (!strcmp(e->getS().c_str(), "sphere")) {
          list<xyzt> tmp_sphere;
          tmp_sphere = e->getSphere();

          for (ss = tmp_sphere.begin(); ss != tmp_sphere.end(); ss++) {
            if (ss->getId().size() > max_len)
              max_len = ss->getId().size();
            if (ss->getL().size() > max_len)
              max_len = ss->getL().size();
          }
        } else if (!strcmp(e->getS().c_str(), "face")) {
          list<xyzt> tmp_face;
          tmp_face = e->getFace();

          for (ss = tmp_face.begin(); ss != tmp_face.end(); ss++) {
            if (ss->getId().size() > max_len)
              max_len = ss->getId().size();
            if (ss->getL().size() > max_len)
              max_len = ss->getL().size();
          }
        } else if (!strcmp(e->getS().c_str(), "object")) {
          list<xyzt> tmp_object;
          tmp_object = e->getObject();

          for (ss = tmp_object.begin(); ss != tmp_object.end(); ss++) {
            if (ss->getId().size() > max_len)
              max_len = ss->getId().size();
            if (ss->getL().size() > max_len)
              max_len = ss->getL().size();
          }
        }
      }
    }

    cerr << "max_length: " << max_len << endl;

    // write series
    for (int i = 0; i < tpsize; i++) {
      std::ostringstream so;
      so << i;
      string sdir = "/data/";
      sdir.append(so.str());

      // create series dir
      sdir_id = H5Gcreate2(file_id, sdir.c_str(), H5P_DEFAULT, H5P_DEFAULT,
                           H5P_DEFAULT);

      // create object dir
      string odir = sdir;
      odir.append("/object");
      odir_id = H5Gcreate2(file_id, odir.c_str(), H5P_DEFAULT, H5P_DEFAULT,
                           H5P_DEFAULT);

      // create feature dir
      //      if (fnn != 0) {

      //      string fdir = sdir;
      //      fdir.append("/feature");
      //      fdir_id = H5Gcreate2(file_id, fdir.c_str(), H5P_DEFAULT,
      //      H5P_DEFAULT, H5P_DEFAULT);

      //      }

      list<objectentity> tmp_oe = ser[i].getOE();
      for (e = tmp_oe.begin(); e != tmp_oe.end(); e++) {
        std::ostringstream oc;
        int on = e->getON();
        oc << on;
        string oname;
        oname.clear();
        oname.append(oc.str());

        if (!strcmp(e->getS().c_str(), "point")) {
          list<xyzt> tmp_point;
          tmp_point = e->getPoint();

          int flaglabel = 0;
          for (ss = tmp_point.begin(); ss != tmp_point.end(); ss++) {
            if (ss->getL().size() > 0) {
              flaglabel = 1;
              break;
            }
          }

          if (max_len < 8) {
            object5 objects[tmp_point.size()];
            int cnt = 0;
            for (ss = tmp_point.begin(); ss != tmp_point.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              //            objects[cnt].ID = ss->getId();
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              //            objects[cnt].entity = ss->getE();
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());
              //            objects[cnt].label = ss->getL();

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object5);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object5));
            H5Tinsert(so_tid, "ID", HOFFSET(object5, ID), strname8.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object5, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object5, z), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);

          } else {
            object16 objects[tmp_point.size()];
            int cnt = 0;
            for (ss = tmp_point.begin(); ss != tmp_point.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              //            objects[cnt].ID = ss->getId();
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              //            objects[cnt].entity = ss->getE();
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());
              //            objects[cnt].label = ss->getL();

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object16);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object16));
            H5Tinsert(so_tid, "ID", HOFFSET(object16, ID), strname16.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object16, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object16, z), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);
          }

        } else if (!strcmp(e->getS().c_str(), "line")) {
          list<xyzt> tmp_line;
          tmp_line = e->getLine();

          int flaglabel = 0;
          for (ss = tmp_line.begin(); ss != tmp_line.end(); ss++) {
            if (ss->getL().size() > 0) {
              flaglabel = 1;
              break;
            }
          }

          if (max_len < 8) {
            object5 objects[tmp_line.size()];
            int cnt = 0;
            for (ss = tmp_line.begin(); ss != tmp_line.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object5);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object5));
            H5Tinsert(so_tid, "ID", HOFFSET(object5, ID), strname8.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object5, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object5, seqID), H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object5, z), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object5, seqID), H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object5, seqID), H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);

          } else {
            object16 objects[tmp_line.size()];
            int cnt = 0;
            for (ss = tmp_line.begin(); ss != tmp_line.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object16);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object16));
            H5Tinsert(so_tid, "ID", HOFFSET(object16, ID), strname16.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object16, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object16, seqID),
                        H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object16, z), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object16, seqID),
                        H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object16, seqID),
                        H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);
          }

        } else if (!strcmp(e->getS().c_str(), "circle")) {
          list<xyzt> tmp_circle;
          tmp_circle = e->getCircle();

          int flaglabel = 0;
          for (ss = tmp_circle.begin(); ss != tmp_circle.end(); ss++) {
            if (ss->getL().size() > 0) {
              flaglabel = 1;
              break;
            }
          }

          if (max_len < 8) {
            object5 objects[tmp_circle.size()];
            int cnt = 0;
            for (ss = tmp_circle.begin(); ss != tmp_circle.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object5);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object5));
            H5Tinsert(so_tid, "ID", HOFFSET(object5, ID), strname8.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object5, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object5, z), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object5, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object5, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object5, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);

          } else {
            object16 objects[tmp_circle.size()];
            int cnt = 0;
            for (ss = tmp_circle.begin(); ss != tmp_circle.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object16);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object16));
            H5Tinsert(so_tid, "ID", HOFFSET(object16, ID), strname16.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object16, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object16, z), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object16, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object16, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object16, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);
          }

        } else if (!strcmp(e->getS().c_str(), "sphere")) {
          list<xyzt> tmp_sphere;
          tmp_sphere = e->getSphere();

          int flaglabel = 0;
          for (ss = tmp_sphere.begin(); ss != tmp_sphere.end(); ss++) {
            if (ss->getL().size() > 0) {
              flaglabel = 1;
              //              cerr << "label exists" << endl;
              break;
            }
          }

          if (max_len < 8) {
            object5 objects[tmp_sphere.size()];
            int cnt = 0;
            for (ss = tmp_sphere.begin(); ss != tmp_sphere.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object5);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object5));
            H5Tinsert(so_tid, "ID", HOFFSET(object5, ID), strname8.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object5, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object5, z), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object5, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object5, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object5, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);

          } else {
            object16 objects[tmp_sphere.size()];
            int cnt = 0;
            for (ss = tmp_sphere.begin(); ss != tmp_sphere.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object16);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object16));
            H5Tinsert(so_tid, "ID", HOFFSET(object16, ID), strname16.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object16, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object16, z), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object16, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object16, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "radius", HOFFSET(object16, r),
                        H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);
          }

        } else if (!strcmp(e->getS().c_str(), "face")) {
          list<xyzt> tmp_face;
          tmp_face = e->getFace();

          int flaglabel = 0;
          for (ss = tmp_face.begin(); ss != tmp_face.end(); ss++) {
            if (ss->getL().size() > 0) {
              flaglabel = 1;
              break;
            }
          }

          if (max_len < 8) {
            object5 objects[tmp_face.size()];
            int cnt = 0;
            for (ss = tmp_face.begin(); ss != tmp_face.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object5);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object5));
            H5Tinsert(so_tid, "ID", HOFFSET(object5, ID), strname8.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object5, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object5, seqID), H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object5, z), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object5, seqID), H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object5, y), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object5, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object5, seqID), H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object5, x), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                          strname8.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);

          } else {
            object16 objects[tmp_face.size()];
            int cnt = 0;
            for (ss = tmp_face.begin(); ss != tmp_face.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object16);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object16));
            H5Tinsert(so_tid, "ID", HOFFSET(object16, ID), strname16.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object16, t), H5T_NATIVE_DOUBLE);

            if (tmp_dim == 3) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object16, seqID),
                        H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "z", HOFFSET(object16, z), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 2) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object16, seqID),
                        H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              H5Tinsert(so_tid, "y", HOFFSET(object16, y), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 1) {
              H5Tinsert(so_tid, "entity", HOFFSET(object16, entity),
                        strname8.getId());
              H5Tinsert(so_tid, "sID", HOFFSET(object16, seqID),
                        H5T_NATIVE_INT);
              H5Tinsert(so_tid, "x", HOFFSET(object16, x), H5T_NATIVE_DOUBLE);
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            } else if (tmp_dim == 0) {
              if (flaglabel == 1) {
                H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                          strname16.getId());
              }
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);
          }

        } else if (!strcmp(e->getS().c_str(), "object")) {
          list<xyzt> tmp_object;
          tmp_object = e->getObject();

          int flaglabel = 0;
          for (ss = tmp_object.begin(); ss != tmp_object.end(); ss++) {
            if (ss->getL().size() > 0) {
              flaglabel = 1;
              //              cerr << "label exists" << endl;
              break;
            }
          }

          if (max_len < 8) {
            object5 objects[tmp_object.size()];
            int cnt = 0;
            for (ss = tmp_object.begin(); ss != tmp_object.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object5);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object5));
            H5Tinsert(so_tid, "ID", HOFFSET(object5, ID), strname8.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object5, t), H5T_NATIVE_DOUBLE);

            if (flaglabel == 1) {
              H5Tinsert(so_tid, "label", HOFFSET(object5, label),
                        strname8.getId());
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);

          } else {
            object16 objects[tmp_object.size()];
            int cnt = 0;
            for (ss = tmp_object.begin(); ss != tmp_object.end(); ss++) {
              strcpy(objects[cnt].ID, ss->getId().c_str());
              objects[cnt].t = ss->getT();
              strcpy(objects[cnt].entity, ss->getE().c_str());
              objects[cnt].seqID = ss->getSId();
              objects[cnt].x = ss->getX();
              objects[cnt].y = ss->getY();
              objects[cnt].z = ss->getZ();
              objects[cnt].r = ss->getR();
              strcpy(objects[cnt].label, ss->getL().c_str());

              cnt++;
            }

            dim[0] = sizeof(objects) / sizeof(object16);
            rank = sizeof(dim) / sizeof(hsize_t);
            so_tid = H5Tcreate(H5T_COMPOUND, sizeof(object16));
            H5Tinsert(so_tid, "ID", HOFFSET(object16, ID), strname16.getId());
            H5Tinsert(so_tid, "t", HOFFSET(object16, t), H5T_NATIVE_DOUBLE);

            if (flaglabel == 1) {
              H5Tinsert(so_tid, "label", HOFFSET(object16, label),
                        strname16.getId());
            }

            so_sid = H5Screate_simple(rank, dim, NULL);
            so_id =
                H5Dcreate1(odir_id, oname.c_str(), so_tid, so_sid, H5P_DEFAULT);
            status =
                H5Dwrite(so_id, so_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, objects);
            H5Dclose(so_id);
            H5Sclose(so_sid);
            H5Tclose(so_tid);
          }
        }
      }

      if (fnn != 0) {

        string fdir = sdir;
        fdir.append("/feature");
        fdir_id = H5Gcreate2(file_id, fdir.c_str(), H5P_DEFAULT, H5P_DEFAULT,
                             H5P_DEFAULT);

        list<property> tmp_pr = ser[i].getPR();
        for (pp = tmp_pr.begin(); pp != tmp_pr.end(); pp++) {
          std::ostringstream fc;
          int fn = pp->getON();
          fc << fn;
          string fname;
          fname.clear();
          fname.append(fc.str());

          list<pelement> tmp_pele;
          tmp_pele = pp->getPL();

          if (max_len < 8) {
            feature5 features[tmp_pele.size()];
            int cnt2 = 0;
            for (sss = tmp_pele.begin(); sss != tmp_pele.end(); sss++) {
              strcpy(features[cnt2].ID, sss->getId().c_str());
              features[cnt2].fID = sss->getFId();
              features[cnt2].value = sss->getVal();

              cnt2++;
            }

            dim[0] = sizeof(features) / sizeof(feature5);
            rank = sizeof(dim) / sizeof(hsize_t);
            sf_tid = H5Tcreate(H5T_COMPOUND, sizeof(feature5));
            H5Tinsert(sf_tid, "ID", HOFFSET(feature5, ID), strname8.getId());
            H5Tinsert(sf_tid, "fID", HOFFSET(feature5, fID), H5T_NATIVE_INT);
            H5Tinsert(sf_tid, "value", HOFFSET(feature5, value),
                      H5T_NATIVE_DOUBLE);

            sf_sid = H5Screate_simple(rank, dim, NULL);
            sf_id =
                H5Dcreate1(fdir_id, fname.c_str(), sf_tid, sf_sid, H5P_DEFAULT);
            status = H5Dwrite(sf_id, sf_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                              features);
            H5Dclose(sf_id);
            H5Sclose(sf_sid);
            H5Tclose(sf_tid);
          } else {
            feature16 features[tmp_pele.size()];
            int cnt2 = 0;
            for (sss = tmp_pele.begin(); sss != tmp_pele.end(); sss++) {
              strcpy(features[cnt2].ID, sss->getId().c_str());
              features[cnt2].fID = sss->getFId();
              features[cnt2].value = sss->getVal();

              cnt2++;
            }

            dim[0] = sizeof(features) / sizeof(feature16);
            rank = sizeof(dim) / sizeof(hsize_t);
            sf_tid = H5Tcreate(H5T_COMPOUND, sizeof(feature16));
            H5Tinsert(sf_tid, "ID", HOFFSET(feature16, ID), strname16.getId());
            H5Tinsert(sf_tid, "fID", HOFFSET(feature16, fID), H5T_NATIVE_INT);
            H5Tinsert(sf_tid, "value", HOFFSET(feature16, value),
                      H5T_NATIVE_DOUBLE);

            sf_sid = H5Screate_simple(rank, dim, NULL);
            sf_id =
                H5Dcreate1(fdir_id, fname.c_str(), sf_tid, sf_sid, H5P_DEFAULT);
            status = H5Dwrite(sf_id, sf_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                              features);
            H5Dclose(sf_id);
            H5Sclose(sf_sid);
            H5Tclose(sf_tid);
          }
        }

        H5Gclose(fdir_id);
      }

      H5Gclose(odir_id);
      H5Gclose(sdir_id);
    }

    list<fromto> tmp_ft;
    tmp_ft = ti.getFT();

    int nfp = tmp_ft.size();

    if (nfp != 0) {

      //      cerr << tmp_ft.size() << endl;
      if (max_len < 8) {
        trackinfo5 tracks[tmp_ft.size()];
        int cnt3 = 0;
        list<fromto>::iterator ift;
        for (ift = tmp_ft.begin(); ift != tmp_ft.end(); ift++) {
          strcpy(tracks[cnt3].from, ift->getFrom().c_str());
          strcpy(tracks[cnt3].to, ift->getTo().c_str());

          cnt3++;
        }

        dim[0] = sizeof(tracks) / sizeof(trackinfo5);
        rank = sizeof(dim) / sizeof(hsize_t);
        si_tid = H5Tcreate(H5T_COMPOUND, sizeof(trackinfo5));
        H5Tinsert(si_tid, "from", HOFFSET(trackinfo5, from), strname8.getId());
        H5Tinsert(si_tid, "to", HOFFSET(trackinfo5, to), strname8.getId());

        si_sid = H5Screate_simple(rank, dim, NULL);
        si_id = H5Dcreate1(data_id, "trackInfo", si_tid, si_sid, H5P_DEFAULT);
        status = H5Dwrite(si_id, si_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, tracks);

        H5Dclose(si_id);
        H5Sclose(si_sid);
        H5Tclose(si_tid);

      } else {
        trackinfo16 tracks[tmp_ft.size()];
        int cnt3 = 0;
        list<fromto>::iterator ift;
        for (ift = tmp_ft.begin(); ift != tmp_ft.end(); ift++) {
          strcpy(tracks[cnt3].from, ift->getFrom().c_str());
          strcpy(tracks[cnt3].to, ift->getTo().c_str());

          cnt3++;
        }

        dim[0] = sizeof(tracks) / sizeof(trackinfo16);
        rank = sizeof(dim) / sizeof(hsize_t);
        si_tid = H5Tcreate(H5T_COMPOUND, sizeof(trackinfo16));
        H5Tinsert(si_tid, "from", HOFFSET(trackinfo16, from),
                  strname16.getId());
        H5Tinsert(si_tid, "to", HOFFSET(trackinfo16, to), strname16.getId());

        si_sid = H5Screate_simple(rank, dim, NULL);
        si_id = H5Dcreate1(data_id, "trackInfo", si_tid, si_sid, H5P_DEFAULT);
        status = H5Dwrite(si_id, si_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, tracks);

        H5Dclose(si_id);
        H5Sclose(si_sid);
        H5Tclose(si_tid);
      }
    }

    status = H5Fclose(file_id);
    /*
        for (int i = 0; i < tpsize; i++) {
          cerr << "time point: " << i << endl;
    //      cerr << ser[i].getOE().size() << endl;
          list<objectentity> tmp_oe = ser[i].getOE();
          for (e = tmp_oe.begin(); e != tmp_oe.end(); e++) {
            cerr << e->getSphere().size() << endl;
            if (!strcmp(e->getS().c_str(), "line")) {
              list<xyzt> tmp_line;
              tmp_line = e->getLine();
              for (ss = tmp_line.begin(); ss != tmp_line.end(); ss++) {
                cout << ss->getId() << " " << ss->getX() << " " << ss->getY() <<
    " " << ss->getZ() << " " << ss->getSId() << " " << ss->getL() << " " <<
    ss->getE() << endl;
              }
            }
            if (!strcmp(e->getS().c_str(), "sphere")) {
              list<xyzt> tmp_sphere;
              tmp_sphere = e->getSphere();
              for (ss = tmp_sphere.begin(); ss != tmp_sphere.end(); ss++) {
                cout << ss->getId() << " " << ss->getX() << " " << ss->getY() <<
    " " << ss->getZ() << " " << ss->getR() << " " << ss->getL() << " " <<
    ss->getE() << endl;
              }
            }
          }
        }
    */

  }

  catch (const xml_schema::exception &e) {
    cerr << e << endl;
    return 1;
  }
}
