﻿using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Text;

namespace GreeBlynkBridge.Gree
{
    class RequestPackInfo
    {
        [JsonProperty("t")]
        public string Type { get; set; }

        [JsonProperty("uid", NullValueHandling=NullValueHandling.Ignore)]
        public int? UID { get; set; }

        [JsonProperty("mac")]
        public string MAC { get; set; }

    }
}
