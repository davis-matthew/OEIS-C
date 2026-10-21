/*
 * A275405  Number of 6Xn 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Empirical recurrence of order 16 for A275405:
   a(n) = 2*a(n-1) +28*a(n-2)+137*a(n-3)+222*a(n-4)-178*a(n-5)-1692*a(n-6)-272*a(n-7)
          +217*a(n-8)+3612*a(n-9)-2256*a(n-10)+1680*a(n-11)-1648*a(n-12)+1472*a(n-13)
          -192*a(n-14)+512*a(n-15)-256*a(n-16)  for n>17
   Initial values:
   a(1)=24, a(2)=129, a(3)=1017, a(4)=8122, a(5)=65045, a(6)=518049, a(7)=4142271,
   a(8)=33102260, a(9)=264412286, a(10)=2112680676, a(11)=16879669489,
   a(12)=134861200228, a(13)=1077489437845, a(14)=8608751763524,
   a(15)=68780667761705, a(16)=549531541049837, a(17)=4390551455487162.
*/

seq_t a(idx_t n)
{
    /* 17 initial values (indices 1..17) */
    static const seq_t init[17] = {
        24ULL,
        129ULL,
        1017ULL,
        8122ULL,
        65045ULL,
        518049ULL,
        4142271ULL,
        33102260ULL,
        264412286ULL,
        2112680676ULL,
        16879669489ULL,
        134861200228ULL,
        1077489437845ULL,
        8608751763524ULL,
        68780667761705ULL,
        549531541049837ULL,
        4390551455487162ULL
    };

    if (n <= 17) {
        return init[n - 1];
    }

    /* Prepare the sliding window for the last 16 computed terms.
       prev[0] holds a(n-16), prev[15] holds a(n-1) during iterations. */
    seq_t prev[16];
    for (idx_t j = 0; j < 16; ++j) {
        prev[j] = init[j + 1];   /* start with a(2) .. a(17) for n=18 */
    }

    seq_t last = 0;
    for (idx_t i = 18; i <= n; ++i) {
        seq_t pos = 0, neg = 0;

        /* Positive coefficients (indices 15,14,13,12,8,7,5,3,1) */
        pos += prev[15] * 2;     /* a(n-1) */
        pos += prev[14] * 28;    /* a(n-2) */
        pos += prev[13] * 137;   /* a(n-3) */
        pos += prev[12] * 222;   /* a(n-4) */
        pos += prev[8]  * 217;   /* a(n-8) */
        pos += prev[7]  * 3612;  /* a(n-9) */
        pos += prev[5]  * 1680;  /* a(n-11) */
        pos += prev[3]  * 1472;  /* a(n-13) */
        pos += prev[1]  * 512;   /* a(n-15) */

        /* Negative coefficients (indices 11,10,9,6,4,2,0) */
        neg += prev[11] * 178;   /* a(n-5) */
        neg += prev[10] * 1692;  /* a(n-6) */
        neg += prev[9]  * 272;   /* a(n-7) */
        neg += prev[6]  * 2256;  /* a(n-10) */
        neg += prev[4]  * 1648;  /* a(n-12) */
        neg += prev[2]  * 192;   /* a(n-14) */
        neg += prev[0]  * 256;   /* a(n-16) */

        seq_t curr = pos - neg;  /* pos >= neg by construction */

        /* Shift window: drop a(n-16), keep a(n-15)..a(n-1), insert a(n) */
        for (idx_t j = 0; j < 15; ++j) {
            prev[j] = prev[j + 1];
        }
        prev[15] = curr;

        last = curr;
    }
    return last;
}
