/* 
 * ARDUINO   MCP4802       
 *           1 VDD         
 * D2        2 CS          8 7 6 5
 * D13       3 SCK         |-----|
 * D11       4 SDI         D     |
 *           5 LDAC        |_____|
 *           6 VOUTB       1 2 3 4
 *           7 GND
 *           8 VOUTA
 */
#include <SPI.h>

#define CHANNEL_A 0x00
#define CHANNEL_B 0x80
#define GAIN_1 0x20
#define GAIN_2 0x00
#define MODE_SHUTDOWN 0x00
#define MODE_ACTIVE 0x10

word channelA = (CHANNEL_A | GAIN_1 | MODE_ACTIVE) << 8;

void setup(){
  SPI.begin();
  pinMode(2, OUTPUT);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
}

unsigned char SINTABLE[256] = {
128, 
129, 
130, 
130, 
131, 
132, 
133, 
133, 
134, 
135, 
136, 
137, 
137, 
138, 
139, 
140, 
140, 
141, 
142, 
143, 
144, 
144, 
145, 
146, 
147, 
147, 
148, 
149, 
150, 
150, 
151, 
152, 
153, 
154, 
154, 
155, 
156, 
157, 
157, 
158, 
159, 
160, 
160, 
161, 
162, 
163, 
163, 
164, 
165, 
166, 
166, 
167, 
168, 
169, 
169, 
170, 
171, 
172, 
172, 
173, 
174, 
174, 
175, 
176, 
177, 
177, 
178, 
179, 
179, 
180, 
181, 
182, 
182, 
183, 
184, 
184, 
185, 
186, 
186, 
187, 
188, 
189, 
189, 
190, 
191, 
191, 
192, 
193, 
193, 
194, 
195, 
195, 
196, 
197, 
197, 
198, 
199, 
199, 
200, 
200, 
201, 
202, 
202, 
203, 
204, 
204, 
205, 
206, 
206, 
207, 
207, 
208, 
209, 
209, 
210, 
210, 
211, 
212, 
212, 
213, 
213, 
214, 
214, 
215, 
216, 
216, 
217, 
217, 
218, 
218, 
219, 
219, 
220, 
221, 
221, 
222, 
222, 
223, 
223, 
224, 
224, 
225, 
225, 
226, 
226, 
227, 
227, 
228, 
228, 
229, 
229, 
230, 
230, 
230, 
231, 
231, 
232, 
232, 
233, 
233, 
234, 
234, 
234, 
235, 
235, 
236, 
236, 
237, 
237, 
237, 
238, 
238, 
239, 
239, 
239, 
240, 
240, 
240, 
241, 
241, 
241, 
242, 
242, 
242, 
243, 
243, 
243, 
244, 
244, 
244, 
245, 
245, 
245, 
246, 
246, 
246, 
246, 
247, 
247, 
247, 
248, 
248, 
248, 
248, 
249, 
249, 
249, 
249, 
250, 
250, 
250, 
250, 
250, 
251, 
251, 
251, 
251, 
251, 
252, 
252, 
252, 
252, 
252, 
252, 
253, 
253, 
253, 
253, 
253, 
253, 
253, 
254, 
254, 
254, 
254, 
254, 
254, 
254, 
254, 
254, 
254, 
254, 
255, 
255, 
255, 
255, 
255, 
255, 
255, 
255, 
255, 
255, 
255, 
255, 
255, 
255
};

void loop() {
  word payload; // payload = channelA | ((x << 4) & 0x0FFF);
  for(word x=0; x<256; x++) {
    payload = channelA | ((x << 4) & 0x0FFF);
    digitalWrite(2, LOW);
    SPI.transfer(highByte(payload));
    SPI.transfer(lowByte(payload));
    digitalWrite(2, HIGH);
  }
//  for(word x=0; x<256; x++) {
//    payload = channelA | ((SINTABLE[x] << 4) & 0x0FFF);
//    digitalWrite(2, LOW);
//    SPI.transfer(highByte(payload));
//    SPI.transfer(lowByte(payload));
//    digitalWrite(2, HIGH);
//  }
//  for(word x=0; x<256; x++) {
//    payload = channelA | ((SINTABLE[255-x] << 4) & 0x0FFF);
//    digitalWrite(2, LOW);
//    SPI.transfer(highByte(payload));
//    SPI.transfer(lowByte(payload));
//    digitalWrite(2, HIGH);
//  }
//  for(word x=0; x<256; x++) {
//    payload = channelA | ((256-SINTABLE[x] << 4) & 0x0FFF);
//    digitalWrite(2, LOW);
//    SPI.transfer(highByte(payload));
//    SPI.transfer(lowByte(payload));
//    digitalWrite(2, HIGH);
//  }
//  for(word x=0; x<256; x++) {
//    payload = channelA | ((256-SINTABLE[255-x] << 4) & 0x0FFF);
//    digitalWrite(2, LOW);
//    SPI.transfer(highByte(payload));
//    SPI.transfer(lowByte(payload));
//    digitalWrite(2, HIGH);
//  }
}
