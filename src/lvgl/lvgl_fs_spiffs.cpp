#include "lvgl_fs_spiffs.h"
#include <LittleFS.h>
#include <FS.h>

// File descriptor structure
typedef struct
{
  File file;
  bool is_open;
} spiffs_file_t;

// Open callback
static void *fs_open_cb(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode)
{
  (void)drv; // Unused

  spiffs_file_t *file_p = (spiffs_file_t *)malloc(sizeof(spiffs_file_t));
  if (file_p == NULL)
  {
    return NULL;
  }

  // Initialize the File object properly before opening
  new (&file_p->file) fs::File();

  const char *mode_str = (mode == LV_FS_MODE_WR) ? "w" : "r";
  fs::File opened_file = LittleFS.open(path, mode_str);

  if (opened_file)
  {
    file_p->file = opened_file;
    file_p->is_open = true;
    return file_p;
  }
  else
  {
    free(file_p);
    return NULL;
  }
}

// Close callback
static lv_fs_res_t fs_close_cb(lv_fs_drv_t *drv, void *file_p)
{
  (void)drv; // Unused

  spiffs_file_t *f = (spiffs_file_t *)file_p;
  if (f->is_open)
  {
    f->file.close();
  }
  free(f);
  return LV_FS_RES_OK;
}

// Read callback
static lv_fs_res_t fs_read_cb(lv_fs_drv_t *drv, void *file_p, void *buf, uint32_t btr, uint32_t *br)
{
  (void)drv; // Unused

  spiffs_file_t *f = (spiffs_file_t *)file_p;
  if (!f->is_open)
  {
    *br = 0;
    return LV_FS_RES_FS_ERR;
  }

  *br = f->file.read((uint8_t *)buf, btr);
  return (*br > 0) ? LV_FS_RES_OK : LV_FS_RES_FS_ERR;
}

// Seek callback
static lv_fs_res_t fs_seek_cb(lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence)
{
  (void)drv; // Unused

  spiffs_file_t *f = (spiffs_file_t *)file_p;
  if (!f->is_open)
    return LV_FS_RES_FS_ERR;

  SeekMode mode = SeekSet;
  if (whence == LV_FS_SEEK_CUR)
    mode = SeekCur;
  else if (whence == LV_FS_SEEK_END)
    mode = SeekEnd;

  return f->file.seek(pos, mode) ? LV_FS_RES_OK : LV_FS_RES_FS_ERR;
}

// Tell callback
static lv_fs_res_t fs_tell_cb(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p)
{
  (void)drv; // Unused

  spiffs_file_t *f = (spiffs_file_t *)file_p;
  if (!f->is_open)
    return LV_FS_RES_FS_ERR;

  *pos_p = f->file.position();
  return LV_FS_RES_OK;
}

// Initialize LittleFS filesystem driver for LVGL
void lvgl_fs_spiffs_init()
{
  // Initialize LittleFS
  if (!LittleFS.begin(true))
  {
    Serial.println("LittleFS mount failed");
    return;
  }

  // Register LittleFS driver with LVGL
  static lv_fs_drv_t fs_drv;
  lv_fs_drv_init(&fs_drv);

  fs_drv.letter = 'S'; // Drive letter 'S:' for filesystem
  fs_drv.cache_size = 0;

  fs_drv.open_cb = fs_open_cb;
  fs_drv.close_cb = fs_close_cb;
  fs_drv.read_cb = fs_read_cb;
  fs_drv.seek_cb = fs_seek_cb;
  fs_drv.tell_cb = fs_tell_cb;

  lv_fs_drv_register(&fs_drv);
}
