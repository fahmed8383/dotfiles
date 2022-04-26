// import neccesary packages
const axios = require("axios");
const secrets = require("./secrets.json");

// list of streamers we want to get info about
const streamers = ["xqcow", "loltyler1"];

// main function
async function main() {

    // if we cannot access the internet then this file will print errors into conky.
    // thus exit if cannot access internet.
    await axios.get('https://yahoo.com/')
        .catch((error) => {
            if(error.code == 'ENOTFOUND'){
                console.log("No Internet Access")
                process.exit()
            }
        });

    token = await getTwitchAccessToken();
    await getStreams(token);
    return
}

// get access token to make twitch api calls
async function getTwitchAccessToken() {
    req = await axios.post("https://id.twitch.tv/oauth2/token?client_id="+secrets.twitch_client_id+"&client_secret="+secrets.twitch_client_secret+"&grant_type=client_credentials");
    res = await req.data;
    return res.access_token;
}

// loop through streamers to get information about them using twitch api
async function getStreams(token) {

    // keep count of how many streamers we are displaying
    count = 0;

    // loop through all streamers
    for(i = 0; i < streamers.length; i++){

        // if we are already displaying 6 streamers, don't display anymore
        if(count == 6){
            break;
        }

        // make api request to get information about the streamer
        req = await axios.get('https://api.twitch.tv/helix/streams?user_login='+streamers[i], {
            headers: {
                "client-id": secrets.twitch_client_id,
                "Authorization": 'Bearer ' + token
            }
        });
        
        // if api call returns data, that means the streamer is live, display the stream
        res = await req.data;
        if (res.data.length != 0){
            console.log("${font :BOLD:pixelsize=16}"+streamers[i]+"${font}");
            console.log("${font :::pixelsize=10}"+res.data[0].game_name+"${font}");
            count++;
        }
    }
}

main();
