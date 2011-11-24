/* Copyright (c) 2011, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA */


#include "zgtids.h"


#ifdef HAVE_GTID


//const int Gtid_specification::MAX_TEXT_LENGTH;


enum_return_status Gtid_specification::parse(Sid_map *sid_map, const char *text)
{
  DBUG_ENTER("Gtid_specification::parse");
  if (text == NULL || strcmp(text, "AUTOMATIC") == 0)
  {
    type= AUTOMATIC_GROUP;
    gtid.sidno= 0;
    gtid.gno= 0;
  }
  else if (strcmp(text, "ANONYMOUS") == 0)
  {
    type= ANONYMOUS_GROUP;
    gtid.sidno= 0;
    gtid.gno= 0;
  }
  else
  {
    PROPAGATE_REPORTED_ERROR(gtid.parse(sid_map, text));
    type= GTID_GROUP;
  }
  RETURN_OK;
};


int Gtid_specification::to_string(const rpl_sid *sid, char *buf) const
{
  DBUG_ENTER("Gtid_specification::to_string(char*)");
  switch (type)
  {
  case AUTOMATIC_GROUP:
    strcpy(buf, "AUTOMATIC");
    DBUG_RETURN(9);
  case ANONYMOUS_GROUP:
    strcpy(buf, "ANONYMOUS");
    DBUG_RETURN(9);
  case GTID_GROUP:
    DBUG_RETURN(gtid.to_string(sid, buf));
  case INVALID_GROUP:
    DBUG_ASSERT(0);
  }
  DBUG_ASSERT(0);
  DBUG_RETURN(0);
}


int Gtid_specification::to_string(const Sid_map *sid_map, char *buf) const
{
  return to_string(type == GTID_GROUP ?
                   sid_map->sidno_to_sid(gtid.sidno) : NULL,
                   buf);
}


enum_group_type Gtid_specification::get_type(const char *text)
{
  DBUG_ENTER("Gtid_specification::is_valid");
  DBUG_ASSERT(text != NULL);
  if (strcmp(text, "AUTOMATIC") == 0)
    DBUG_RETURN(AUTOMATIC_GROUP);
  else if (strcmp(text, "ANONYMOUS") == 0)
    DBUG_RETURN(ANONYMOUS_GROUP);
  else
    DBUG_RETURN(Gtid::is_valid(text) ? GTID_GROUP : INVALID_GROUP);
}


#endif /* HAVE_GTID */
