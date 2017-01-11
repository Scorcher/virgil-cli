/**
 * Copyright (C) 2015-2016 Virgil Security Inc.
 *
 * Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>
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
 */

#include <cli/argument/ArgumentTransformer.h>

#include <cli/api/api.h>
#include <cli/model/Recipient.h>
#include <cli/model/SecureKey.h>
#include <cli/model/DecryptCredentials.h>
#include <cli/model/Token.h>
#include <cli/error/ArgumentError.h>

using cli::argument::ArgumentTransformer;
using cli::model::Recipient;
using cli::model::DecryptCredentials;
using cli::model::Token;
using cli::model::SecureKey;
using cli::error::ArgumentValueError;
using cli::Crypto;


std::vector<std::unique_ptr<Recipient>> ArgumentTransformer<Recipient>::transform() const {
    std::vector<std::unique_ptr<Recipient>> result;
    for (const auto& recipientId : argumentValueList_) {
        Token recipientToken(recipientId);
        result.push_back(Recipient::create(recipientToken));
    }
    return result;
}

std::unique_ptr<SecureKey> ArgumentTransformer<SecureKey>::transform() const {
    return std::make_unique<SecureKey>(Crypto::ByteUtils::stringToBytes(argumentValue_));
}
