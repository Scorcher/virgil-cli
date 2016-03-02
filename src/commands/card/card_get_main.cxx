/**
 * Copyright (C) 2015 Virgil Security Inc.
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

#include <iostream>
#include <string>
#include <stdexcept>

#include <tclap/CmdLine.h>

#include <virgil/sdk/ServicesHub.h>
#include <virgil/sdk/io/Marshaller.h>

#include <cli/version.h>
#include <cli/config.h>
#include <cli/pair.h>
#include <cli/util.h>

namespace vsdk = virgil::sdk;
namespace vcrypto = virgil::crypto;
namespace vcli = virgil::cli;

#ifdef SPLIT_CLI
#define MAIN main
#else
#define MAIN card_get_main
#endif

int MAIN(int argc, char **argv) {
    try {
        std::string description = "Get user's Virgil Card/Cards from the Virgil Keys service\n";

        std::vector <std::string> examples;
        examples.push_back(
                "Get Virgil Card by card-id:\n"
                "virgil card-get -a <card-id>\n");

        examples.push_back(
                "Get Virgil Cars:\n"
                "virgil card-get -a <card-id> -e <public-key-id> -k <private_key>\n");

        std::string descriptionMessage = virgil::cli::getDescriptionMessage(description, examples);

        // Parse arguments.
        TCLAP::CmdLine cmd(descriptionMessage, ' ', virgil::cli_version());

        TCLAP::ValueArg<std::string> outArg("o", "out", "Virgil Card. If omitted stdout is used.",
                false, "", "file");

        TCLAP::ValueArg<std::string> cardIdArg("a", "card-id", "Virgil Card identifier",
                true, "", "arg");

        TCLAP::ValueArg<std::string> publicKeyIdArg("e", "public-key-id", "Public Key identifier\n",
                false, "", "arg");

        TCLAP::ValueArg<std::string> privateKeyArg("k", "private-key", "Private key",
                false, "", "file");

        TCLAP::ValueArg<std::string> privateKeyPassArg("p", "private-key-pass", "Private key pass",
                false, "", "arg");

        cmd.add(privateKeyPassArg);
        cmd.add(privateKeyArg);
        cmd.add(publicKeyIdArg);
        cmd.add(cardIdArg);
        cmd.add(outArg);
        cmd.parse(argc, argv);

        vsdk::ServicesHub servicesHub(VIRGIL_ACCESS_TOKEN);
        if (publicKeyIdArg.isSet() && privateKeyArg.isSet()) {
            std::string pathPrivateKey = privateKeyArg.getValue();
            vcrypto::VirgilByteArray privateKey = vcli::readFileBytes(pathPrivateKey);

            vcrypto::VirgilByteArray privateKeyPass = vcrypto::str2bytes(privateKeyPassArg.getValue());
            vsdk::Credentials credentials(privateKey, privateKeyPass);

            std::vector<vsdk::model::Card> cards = servicesHub.card().get(publicKeyIdArg.getValue(),
                    cardIdArg.getValue(), credentials);
            std::string cardsStr = vsdk::io::cardsToJson(cards, 4);
            vcli::writeBytes(outArg.getValue(), cardsStr);
        } else {
            vsdk::model::Card card = servicesHub.card().get(cardIdArg.getValue());
            std::string cardStr = vsdk::io::Marshaller<vsdk::model::Card>::toJson<4>(card);
            vcli::writeBytes(outArg.getValue(), cardStr);
        }

    } catch (TCLAP::ArgException& exception) {
        std::cerr << "private-key-get. Error: " << exception.error() << " for arg " << exception.argId() << std::endl;
        return EXIT_FAILURE;
    } catch (std::exception& exception) {
        std::cerr << "private-key-get. Error: " << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
