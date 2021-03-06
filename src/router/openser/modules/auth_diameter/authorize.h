/* 
 * $Id: authorize.h,v 1.1.1.1 2005/06/13 16:47:33 bogdan_iancu Exp $
 *
 * Digest Authentication - Diameter support
 *
 * Copyright (C) 2001-2003 FhG Fokus
 *
 * This file is part of openser, a free SIP server.
 *
 * openser is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 * 
 * openser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 * -------
 *  
 *  
 */

#ifndef DIAMETER_AUTHORIZE_H
#define DIAMETER_AUTHORIZE_H

#include "diameter_msg.h"
#include "../../parser/digest/digest_parser.h"
#include "../../parser/hf.h"
#include "defs.h"

int get_uri(struct sip_msg* m, str** uri);

int get_realm(struct sip_msg* m, int hftype, struct sip_uri* u);

auth_result_t pre_auth(struct sip_msg* m, str* realm, int hftype, 
						struct hdr_field** h);

int find_credentials(struct sip_msg* m, str* realm, int hftype, 
						struct hdr_field** h);

int authorize(struct sip_msg* msg, str* realm, int hftype);

int diameter_authorize(struct hdr_field* cred, str* p_method, 
					struct sip_uri uri,	struct sip_uri ruri,
					unsigned int m_id, rd_buf_t *response);

int srv_response(struct sip_msg* msg, rd_buf_t* rb, int hftype);

int send_resp(struct sip_msg* _m, int _code, char* _reason,
					char* _hdr, int _hdr_len);

#endif /* DIAMETER_AUTHORIZE_H */
 
