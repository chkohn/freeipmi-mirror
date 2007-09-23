#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */

#include "bmc-config.h"
#include "bmc-config-common.h"
#include "bmc-config-wrapper.h"
#include "bmc-config-diff.h"
#include "bmc-config-map.h"
#include "bmc-config-sections.h"
#include "bmc-config-validate.h"

static config_err_t
enable_gratuitous_arps_checkout (const struct config_section *section,
				 struct config_keyvalue *kv,
                                 void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  uint8_t enable_arp;
  uint8_t reply_arp;
  config_err_t ret;

  ret = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
						    &enable_arp,
						    &reply_arp);
  if (ret != 0)
    return -1;

  if (!(kv->value = strdup (enable_arp ? "Yes" : "No")))
    {
      perror("strdup");
      return -1;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
enable_gratuitous_arps_commit (const struct config_section *section,
			       const struct config_keyvalue *kv,
                               void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  int ret;
  uint8_t enable_arp;
  uint8_t reply_arp;
  ret = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
						    &enable_arp,
						    &reply_arp);
  if (ret != 0)
    return -1;

  enable_arp = same (kv->value, "yes");

  return set_bmc_lan_conf_bmc_generated_arp_control (state_data,
						     enable_arp,
						     reply_arp);
}

static config_diff_t
enable_gratuitous_arps_diff (const struct config_section *section,
			     const struct config_keyvalue *kv,
                             void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  uint8_t enable_arp;
  uint8_t reply_arp;
  config_err_t rc;
  config_diff_t ret;

  if ((rc = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
                                                        &enable_arp,
                                                        &reply_arp)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  if (enable_arp == same (kv->value, "yes"))
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT; 
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   (enable_arp) ? "Yes" : "No");
    }
  return ret;
}

/* reply */

static config_err_t
enable_arp_response_checkout (const struct config_section *section,
			      struct config_keyvalue *kv,
                              void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  uint8_t enable_arp;
  uint8_t reply_arp;
  config_err_t ret;

  if ((ret = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
                                                         &enable_arp,
                                                         &reply_arp)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (!(kv->value = strdup (reply_arp ? "Yes" : "No")))
    {
      perror("strdup");
      return CONFIG_ERR_FATAL_ERROR;
    }

  return CONFIG_ERR_SUCCESS;
}

static config_err_t
enable_arp_response_commit (const struct config_section *section,
			    const struct config_keyvalue *kv,
                            void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  uint8_t enable_arp;
  uint8_t reply_arp;
  config_err_t ret;
  
  if ((ret = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
                                                         &enable_arp,
                                                         &reply_arp)) != CONFIG_ERR_SUCCESS)
    return ret;

  reply_arp = same (kv->value, "yes");

  return set_bmc_lan_conf_bmc_generated_arp_control (state_data,
						     enable_arp,
						     reply_arp);
}

static config_diff_t
enable_arp_response_diff (const struct config_section *section,
			  const struct config_keyvalue *kv,
                          void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  uint8_t enable_arp;
  uint8_t reply_arp;
  config_err_t rc;
  config_diff_t ret;

  if ((rc = get_bmc_lan_conf_bmc_generated_arp_control (state_data,
                                                        &enable_arp,
                                                        &reply_arp)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  if (reply_arp == same (kv->value, "yes"))
    ret = CONFIG_DIFF_SAME;
  else
    {
      ret = CONFIG_DIFF_DIFFERENT; 
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   (reply_arp) ? "Yes" : "No");
    }

  return ret;
}

static config_err_t
gratuitous_arp_interval_checkout (const struct config_section *section,
				  struct config_keyvalue *kv,
                                  void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  uint8_t interval;
  config_err_t ret;

  if ((ret = get_bmc_lan_conf_gratuitous_arp_interval (state_data,
                                                       &interval)) != CONFIG_ERR_SUCCESS)
    return ret;

  if (asprintf (&kv->value, "%d", interval) < 0)
    {
      perror("asprintf");
      return CONFIG_ERR_FATAL_ERROR;
    }
  return CONFIG_ERR_SUCCESS;
}

static config_err_t
gratuitous_arp_interval_commit (const struct config_section *section,
				const struct config_keyvalue *kv,
                                void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  return set_bmc_lan_conf_gratuitous_arp_interval (state_data,
                                                   atoi (kv->value));
}

static config_diff_t
gratuitous_arp_interval_diff (const struct config_section *section,
			      const struct config_keyvalue *kv,
                              void *arg)
{
  bmc_config_state_data_t *state_data = (bmc_config_state_data_t *)arg;
  uint8_t interval;
  config_err_t rc;
  config_diff_t ret;
  
  if ((rc = get_bmc_lan_conf_gratuitous_arp_interval (state_data,
                                                       &interval)) != CONFIG_ERR_SUCCESS)
    {
      if (rc == CONFIG_ERR_NON_FATAL_ERROR)
        return CONFIG_DIFF_NON_FATAL_ERROR;
      return CONFIG_DIFF_FATAL_ERROR;
    }

  if (interval == atoi (kv->value))
    ret = CONFIG_DIFF_SAME;
  else
    {
      char num[32];
      ret = CONFIG_DIFF_DIFFERENT; 
      sprintf (num, "%d", interval);
      report_diff (section->section_name,
                   kv->key_name,
                   kv->value,
                   num);
    }
  return ret;
}

struct config_section *
bmc_config_lan_conf_misc_section_get (bmc_config_state_data_t *state_data)
{
  struct config_section *lan_conf_misc_section = NULL;
  char *section_comment = 
    "The following miscellaneous configuration options are optionally "
    "implemented by the vendor.  They may not be available your system and "
    "may not be visible below."
    "\n"
    "If set to \"Yes\", \"Enable_Gratuitous_ARPs\" will inform the BMC to "
    "regularly send out Gratuitous ARPs to allow other machines on a "
    "network resolve the BMC's MAC Address.  Many users will want to set "
    "this to \"Yes\" because it offers the easiest way to support BMC IP "
    "Address resolution.  However, it will increase traffic on your "
    "network.  The \"Gratuitous_ARP_Interval\" can be used to set the "
    "period a Gratuitous ARP is always sent."
    "\n"
    "If set to \"Yes\", \"Enable_ARP_Response\" will inform the BMC to"
    "respond to ARP requests from other machines.";

  if (!(lan_conf_misc_section = bmc_config_section_create (state_data, 
                                                           "Lan_Conf_Misc",
                                                           "Lan_Conf_Misc",
                                                           section_comment,
                                                           0)))
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_conf_misc_section,
                                       "Enable_Gratuitous_ARPs",
                                       "Possible values: Yes/No",
                                       0,
                                       enable_gratuitous_arps_checkout,
                                       enable_gratuitous_arps_commit,
                                       enable_gratuitous_arps_diff,
                                       config_yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_conf_misc_section,
                                       "Enable_ARP_Response",
                                       "Possible values: Yes/No",
                                       0,
                                       enable_arp_response_checkout,
                                       enable_arp_response_commit,
                                       enable_arp_response_diff,
                                       config_yes_no_validate) < 0)
    goto cleanup;

  if (bmc_config_section_add_keyvalue (state_data,
                                       lan_conf_misc_section,
                                       "Gratuitous_ARP_Interval",
                                       "Give a number (x 500ms)",
                                       0,
                                       gratuitous_arp_interval_checkout,
                                       gratuitous_arp_interval_commit,
                                       gratuitous_arp_interval_diff,
                                       config_number_range_one_byte) < 0)
    goto cleanup;
  return lan_conf_misc_section;

 cleanup:
  if (lan_conf_misc_section)
    bmc_config_section_destroy(state_data, lan_conf_misc_section);
  return NULL;
}
