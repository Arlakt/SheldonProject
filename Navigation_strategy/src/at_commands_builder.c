#include "at_commands_builder.h"

int convert_float(float a)
{
    if ((a < -1.0) || (a > 1.0))
    {
        fprintf(stderr, "[%s:%d] Error: given float is not in [-1..1]\n", __FILE__, __LINE__);
        return 0;
    }
    else
        return *(int *)(&a);
}

char *at_ref(char *buf, int seq, int control)
{
    if (buf != NULL)
        sprintf(buf, "AT*REF=%d,%d\r", seq, control);
    else
        fprintf(stderr, "[%s:%d] Error: Buffer is null!", __FILE__, __LINE__);
    return buf;
}

char *at_pcmd(char *buf, int seq, pcmd_t pcmd)
{
    if (buf != NULL)
    {
        sprintf(buf, "AT*PCMD=%d,%d,%d,%d,%d,%d\r",
                seq,
                pcmd.progressive,
                convert_float(pcmd.rollTilt),
                convert_float(pcmd.pitchTilt),
                convert_float(pcmd.verticalSpeed),
                convert_float(pcmd.angularSpeed));
    }
    else
        fprintf(stderr, "[%s:%d] Error: Buffer is null!", __FILE__, __LINE__);
    return buf;
}


char *at_ftrim(char *buf, int seq)
{
    if (buf != NULL)
        sprintf(buf, "AT*FTRIM=%d\r", seq);
    else
        fprintf(stderr, "[%s:%d] Error: Buffer is null!", __FILE__, __LINE__);
    return buf;
}

char *at_calib(char *buf, int seq, ardrone_calibration_device_t id)
{
    if (buf != NULL)
        sprintf(buf, "AT*CALIB=%d,%d\r", seq, id);
    else
        fprintf(stderr, "[%s:%d] Error: Buffer is null!", __FILE__, __LINE__);
    return buf;
}

char *at_config(char *buf, int seq, const char *name, const char *value)
{
   if (buf != NULL)
      sprintf(buf, "AT*CONFIG=%d,\"%s\",\"%s\"\r", seq, name, value);
   else
        fprintf(stderr, "[%s:%d] Error: Buffer is null!", __FILE__, __LINE__);
   return buf;
}

char *at_config_ids(char *buf, int seq, const char *sessionId, const char *userId, const char *appId)
{
    if (buf != NULL)
        sprintf(buf, "AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", seq, sessionId, userId, appId);
    else
        fprintf(stderr, "[%s:%d] Error: Buffer is null!", __FILE__, __LINE__);
    return buf;
}

char *at_comwdg(char *buf)
{
    if (buf != NULL)
        sprintf(buf, "AT*COMWDG=1\r");
    else
        fprintf(stderr, "[%s:%d] Error: Buffer is null!", __FILE__, __LINE__);
    return buf;
}
