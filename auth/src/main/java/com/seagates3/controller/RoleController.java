/*
 * COPYRIGHT 2015 SEAGATE LLC
 *
 * THIS DRAWING/DOCUMENT, ITS SPECIFICATIONS, AND THE DATA CONTAINED
 * HEREIN, ARE THE EXCLUSIVE PROPERTY OF SEAGATE TECHNOLOGY
 * LIMITED, ISSUED IN STRICT CONFIDENCE AND SHALL NOT, WITHOUT
 * THE PRIOR WRITTEN PERMISSION OF SEAGATE TECHNOLOGY LIMITED,
 * BE REPRODUCED, COPIED, OR DISCLOSED TO A THIRD PARTY, OR
 * USED FOR ANY PURPOSE WHATSOEVER, OR STORED IN A RETRIEVAL SYSTEM
 * EXCEPT AS ALLOWED BY THE TERMS OF SEAGATE LICENSES AND AGREEMENTS.
 *
 * YOU SHOULD HAVE RECEIVED A COPY OF SEAGATE'S LICENSE ALONG WITH
 * THIS RELEASE. IF NOT PLEASE CONTACT A SEAGATE REPRESENTATIVE
 * http://www.seagate.com/contact
 *
 * Original author:  Arjun Hariharan <arjun.hariharan@seagate.com>
 * Original creation date: 1-Nov-2015
 */

package com.seagates3.controller;

import com.seagates3.dao.DAODispatcher;
import com.seagates3.dao.DAOResource;
import com.seagates3.dao.RoleDAO;
import com.seagates3.exception.DataAccessException;
import com.seagates3.model.Requestor;
import com.seagates3.model.Role;
import com.seagates3.response.ServerResponse;
import com.seagates3.response.generator.xml.RoleResponseGenerator;
import com.seagates3.util.DateUtil;
import java.util.Map;
import org.joda.time.DateTime;

public class RoleController extends AbstractController {
    RoleDAO roleDAO;
    RoleResponseGenerator responseGenerator;

    public RoleController(Requestor requestor,
            Map<String, String> requestBody) {
        super(requestor, requestBody);

        roleDAO = (RoleDAO) DAODispatcher.getResourceDAO(DAOResource.ROLE);
        responseGenerator = new RoleResponseGenerator();
    }

    @Override
    public ServerResponse create() throws DataAccessException {
        Role role = roleDAO.find(requestor.getAccountName(),
                requestBody.get("RoleName"));

        if(role.exists()) {
            return responseGenerator.entityAlreadyExists();
        }

        role.setRolePolicyDoc(requestBody.get("AssumeRolePolicyDocument"));

        if(requestBody.containsKey("path")) {
            role.setPath(requestBody.get("path"));
        } else {
            role.setPath("/");
        }

        role.setCreateDate(DateUtil.toServerResponseFormat(DateTime.now()));

        roleDAO.save(role);

        return responseGenerator.create(role);
    }

    /*
     * TODO
     * Check if the role has policy attached before deleting.
     */
    @Override
    public ServerResponse delete() throws DataAccessException {
        Role role = roleDAO.find(requestor.getAccountName(),
                requestBody.get("RoleName"));

        if(!role.exists()) {
            return responseGenerator.noSuchEntity();
        }

        /*
         * Check if policy is associated with the role.
         */
        roleDAO.delete(role);

        return responseGenerator.delete();
    }

    @Override
    public ServerResponse list() throws DataAccessException {
        String pathPrefix;

        if(requestBody.containsKey("PathPrefix")) {
            pathPrefix = requestBody.get("PathPrefix");
        } else {
            pathPrefix = "/";
        }

        Role[] roleList;
        roleList = roleDAO.findAll(requestor.getAccountName(), pathPrefix);

        if(roleList == null) {
            return responseGenerator.internalServerError();
        }

        return responseGenerator.list(roleList);
    }
}