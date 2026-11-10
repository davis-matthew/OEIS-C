/*
 * A241582  Records in A131644.
 * https://oeis.org/A241582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   records (successive maxima) in A131644, where A131644(m) =
 *            2^A131644(m-1) mod m, A131644(1) = 0; found by iterating A131644
 *            in native 64-bit arithmetic and recording each new maximum
 * Range:     u8 n <= 31, u16 n <= 615, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9763 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t A241582_powmod2(uint64_t e, uint64_t m)
{
    uint64_t result = 1 % m;
    uint64_t b = 2 % m;
    if (m < ((uint64_t)1 << 32)) {
        while (e != 0) {
            if (e & 1) result = (result * b) % m;
            b = (b * b) % m;
            e >>= 1;
        }
    } else {
        while (e != 0) {
            if (e & 1) result = (uint64_t)(((unsigned __int128)result * b) % m);
            b = (uint64_t)(((unsigned __int128)b * b) % m);
            e >>= 1;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    static uint64_t *recs = NULL;
    static idx_t count = 0;
    static idx_t cap = 0;
    static uint64_t pos = 0;   /* index m of last computed A131644 term */
    static uint64_t last = 0;  /* A131644(pos) */
    static uint64_t mx = 0;    /* max so far */

    if (n == 0) return 0;

    if (pos == 0) {
        cap = 1024;
        recs = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
        if (!recs) exit(1);
        recs[0] = 0;
        count = 1;
        pos = 1;
        last = 0;
        mx = 0;
    }

    while (count < n) {
        pos++;
        last = A241582_powmod2(last, pos);
        if (last > mx) {
            mx = last;
            if (count >= cap) {
                idx_t ncap = cap * 2;
                uint64_t *nr = (uint64_t *)realloc(recs, (size_t)ncap * sizeof(uint64_t));
                if (!nr) exit(1);
                recs = nr;
                cap = ncap;
            }
            recs[count++] = last;
        }
    }
    return (seq_t)recs[n - 1];
}
