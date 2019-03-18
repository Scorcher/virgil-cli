package utils

import (
	"io/ioutil"
	"os"
	"os/user"
	"path/filepath"
)

/*
 * Copyright (C) 2015-2019 Virgil Security Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     (1) Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *     (2) Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *     (3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>
 */

import (
	"github.com/pkg/errors"
)

func SaveAppID(appID string) error {

	u, err := user.Current()
	if err != nil {
		return err
	}

	appIDPath := filepath.Join(u.HomeDir, ".virgil_app")

	if _, err := os.Stat(appIDPath); os.IsNotExist(err) {
		if err = os.Mkdir(appIDPath, 0700); err != nil {
			return err
		}
	}

	appIDPath = filepath.Join(appIDPath, "virgil_app")

	if err = ioutil.WriteFile(appIDPath, []byte(appID), 0600); err != nil {
		return err
	}
	return nil
}

func LoadAppID() (appID string, err error) {
	u, err := user.Current()
	if err != nil {
		return "", err
	}

	tokenPath := filepath.Join(u.HomeDir, ".virgil_app")

	if _, err := os.Stat(tokenPath); os.IsNotExist(err) {
		return "", errors.New("virgil_app folder does not exist")
	}

	tokenPath = filepath.Join(tokenPath, "virgil_app")

	if appID, err := ioutil.ReadFile(tokenPath); err != nil {
		return "", err
	} else {
		return string(appID), nil
	}
}

func DeleteAppID() error {
	u, err := user.Current()
	if err != nil {
		return err
	}

	appIDPath := filepath.Join(u.HomeDir, ".virgil_app")

	if _, err := os.Stat(appIDPath); os.IsNotExist(err) {
		return errors.New(".virgil_app directory does not exist")
	}

	appIDPath = filepath.Join(appIDPath, "virgil_app")

	return os.Remove(appIDPath)
}