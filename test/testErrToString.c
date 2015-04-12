#include "sput.h"
#include "dynar.h"

static void testValid(void)
{
    sput_fail_if(strncmp(daErrToString(DA_OK), "DA_OK", strlen("DA_OK")) != 0, "daErrToString should return appropriate error string (DA_OK)");
    sput_fail_if(strncmp(daErrToString(DA_PARAM_ERR | DA_PARAM_NULL), "DA_PARAM_ERR | DA_PARAM_NULL", strlen("DA_PARAM_ERR | DA_PARAM_NULL")) != 0, "daErrToString should return appropriate error string (DA_PARAM_ERR | DA_PARAM_NULL)");
    sput_fail_if(strncmp(daErrToString(DA_PARAM_ERR | DA_UNKNOWN_MODE), "DA_PARAM_ERR | DA_UNKNOWN_MODE", strlen("DA_PARAM_ERR | DA_UNKNOWN_MODE")) != 0, "daErrToString should return appropriate error string (DA_PARAM_ERR | DA_UNKNOWN_MODE)");
    sput_fail_if(strncmp(daErrToString(DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "DA_PARAM_ERR | DA_OUT_OF_BOUNDS", strlen("DA_PARAM_ERR | DA_OUT_OF_BOUNDS")) != 0, "daErrToString should return appropriate error string (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(strncmp(daErrToString(DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT", strlen("DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT")) != 0, "daErrToString should return appropriate error string (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT");
    sput_fail_if(strncmp(daErrToString(DA_FATAL | DA_ENOMEM), "DA_FATAL | DA_ENOMEM", strlen("DA_FATAL | DA_ENOMEM")) != 0, "daErrToString should return appropriate error string (DA_FATAL | DA_ENOMEM)");
    sput_fail_if(strncmp(daErrToString(DA_NOT_FOUND), "DA_NOT_FOUND", strlen("DA_NOT_FOUND")) != 0, "daErrToString should return appropriate error string (DA_NOT_FOUND)");
}

static void testInvalid(void)
{
    sput_fail_if(strncmp(daErrToString(0x123456), "UNKNOWN ERROR", strlen("UNKNOWN ERROR")) != 0, "daErrToString should return UNKNOWN ERROR if the error code is unknown (UNKNOWN ERROR)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daErrToString should return appropriate strings");
    sput_run_test(testValid);

    sput_enter_suite("daErrToString should return error message if the error code is unknown");
    sput_run_test(testInvalid);

    sput_finish_testing();

    return sput_get_return_value();
}