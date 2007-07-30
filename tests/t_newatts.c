#include "hdf5.h"

#define FILENAME "compat.h5"


#ifndef TRUE
#define TRUE 1
#endif /* TRUE */
#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

/*================================================
                       /
     |           |        |      |      | 
     g1          g2       g3     g4     g5
    / \          |        |      |      |
 g1.1 g1.2     dtype1   hlink2  dset2  slink1
  |     |     (dset1)   (dset2)        (dset2)
 dset1 hlink1
       (dset1)
================================================*/

int main(int argc, char *argv[])
{
/* =====  Variables  ===== */

    hid_t fid;		/* file ID */
    hid_t gid;          /* group ID */
    hid_t aid;          /* attribute ID */
    hid_t did;          /* dataset ID */
    hid_t sid;		/* dataspace ID */

    hid_t fapl;		/* file access property list */

    hsize_t dims[2];

    char buf[60];
    int i;

/* =====  Main processing  ===== */

   /* Open File */
    fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_latest_format(fapl, TRUE);
    fid = H5Fopen(FILENAME, H5F_ACC_RDWR, fapl);

   /* Open Group g4 */
    gid = H5Gopen(fid, "/g4");

   /* Open Dataset dset2 */
    did = H5Dopen(gid, "dset2");

   /* Remove old attributes from dset2 */
    for (i = 0; i < 10; i++){
        sprintf(buf, "attr%05d", i);
	H5Adelete2( did, ".", buf, H5P_DEFAULT);
    }

   /* Add attributes to dset2 */
    for (i = 0; i < 10; i++){
        dims[0] = 10;
        sprintf(buf, "attr%05d", i);
        sid = H5Screate_simple(1, dims, NULL);
        aid = H5Acreate (did, buf, H5T_NATIVE_CHAR, sid, H5P_DEFAULT);
        H5Awrite(aid, H5T_NATIVE_CHAR, buf);
        H5Sclose(sid);
        H5Aclose(aid);
    }
    
   /* Close some things */
    H5Dclose(did);
    H5Gclose(gid);

 /* =========================================== */


   /* Close File */
    H5Fclose(fid);

    return 0;

}











