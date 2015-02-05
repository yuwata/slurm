/*****************************************************************************\
 *  grid.c - Functions to manage inter-cluster job IDs
 *****************************************************************************
 *  Copyright (C) 2015 Swiss National Supercomputing Centre
 *  Written by Stephen Trofinoff
 *
 *  This file is part of SLURM, a resource management program.
 *  For details, see <http://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "slurm/slurm_errno.h"
#include "src/common/slurmdbd_defs.h"
#include "src/common/xmalloc.h"

extern int slurmctld_grid_update_request(void *db_conn)
{
	slurmdbd_msg_t  msg;
	int             rc = SLURM_SUCCESS;
	dbd_grid_table_msg_t *ret_table = xmalloc(sizeof(dbd_grid_table_msg_t));
	slurmdbd_msg_t *resp; /* For use with direct call
			       * to slurm_send_recv_slurmdbd_msg */
	resp = xmalloc(sizeof(slurmdbd_msg_t));

	info("Obtaining updated Slurm grid table from the slurmdbd.");

	msg.msg_type     = DBD_GRID_UPDATE_REQUEST;
	resp->data       = ret_table;
	slurm_send_recv_slurmdbd_msg(SLURM_PROTOCOL_VERSION, &msg, resp);

	/* ... The RPC manager catches and processes the results. ... */

	xfree(resp);

	return rc;
}

extern int request_sicp_job_id(void *db_conn, uint32_t* job_id)
{
	slurmdbd_msg_t msg;
	int rc = SLURM_SUCCESS;
	uint32_t new_ic_jobid;
	slurmdbd_msg_t *resp; /* For use with direct call
			       * to slurm_send_recv_slurmdbd_msg */
	/* Alloc space for return SICP job id AND possibly message type value */
	uint32_t *rawmsg =  xmalloc(sizeof(uint32_t) * 3);

	resp = xmalloc(sizeof(slurmdbd_msg_t));
	msg.msg_type     = DBD_SICP_JOB_ID_REQUEST;
	msg.data         = NULL;

	resp->data       = rawmsg;
	rc = slurm_send_recv_slurmdbd_msg(SLURM_PROTOCOL_VERSION, &msg, resp);

	if (slurm_get_debug_flags() & DEBUG_FLAG_SICP) {
		info("SICP--%s--rc = %s", __FUNCTION__,
		     (rc == SLURM_SUCCESS) ? "SLURM_SUCCESS" : "SLURM_FAILURE");
	}

	new_ic_jobid = *(uint32_t*)resp->data;
	(*job_id) = new_ic_jobid;

	/* We now return 0 for the SICP job id if there is a failure.
	 * One possible cause is if all available SICP job id's are
	 * currently in use.
	 */

	if (!new_ic_jobid)
		rc = SLURM_FAILURE;

	if (slurm_get_debug_flags() & DEBUG_FLAG_SICP) {
		info("SICP--%s--rc = %s", __FUNCTION__,
			(rc==SLURM_SUCCESS)?"SLURM_SUCCESS" : "SLURM_FAILURE");
	}
	xfree(resp);

	return rc;
}

extern int request_sicp_jobid_cluster_idx(void *db_conn, uint32_t job_id,
					  int *idx)
{
	slurmdbd_msg_t msg;
	int rc = SLURM_SUCCESS;

	/* Allocate space for the return SICP jobid cluster index
	 * AND possibly the message type value
	 */
	uint32_t* rawmsg =  xmalloc(sizeof(uint32_t) * 3);

	/* For use with direct call to slurm_send_recv_slurmdbd_msg */
	slurmdbd_msg_t *resp;

	resp = xmalloc(sizeof(slurmdbd_msg_t));

	msg.msg_type     = DBD_SICP_JOBID_CLUSTER_IDX_REQUEST;
	msg.data         = &job_id;

	resp->data       = rawmsg;
	rc = slurm_send_recv_slurmdbd_msg(SLURM_PROTOCOL_VERSION, &msg, resp);

	if (slurm_get_debug_flags() & DEBUG_FLAG_SICP) {
		info("SICP--%s--rc = %s", __FUNCTION__,
		     (rc==SLURM_SUCCESS) ? "SLURM_SUCCESS" : "SLURM_FAILURE");
	}

	(*idx) = *(uint32_t*)resp->data;
	if (slurm_get_debug_flags() & DEBUG_FLAG_SICP) {
		if (rc == SLURM_SUCCESS)
			info("SICP--%s--SICP job id %u has cluster index of %d",
			     __FUNCTION__, job_id, *idx);
	}
	xfree(resp);

	return rc;
}
