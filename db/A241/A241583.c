/*
 * A241583  Where records occur in A131644.
 * https://oeis.org/A241583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) = n-th index k where A131644(k) sets a new record
 *            (A131644(1) = 0, A131644(k) = 2^A131644(k-1) mod k); a(1) = 1
 * Range:     u8 n <= 29, u16 n <= 615, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8910 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 2^e mod m with m < 2^32, so products fit in 64 bits */
static uint64_t A241583_pow2mod(uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m, b = 2 % m;
    while (e) {
        if (e & 1u) r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    static idx_t *rec = NULL;
    static idx_t cnt = 0, cap = 0;
    static uint64_t i = 1, v = 0, mx = 0;

    if (n == 0) return 0;
    if (rec == NULL) {
        cap = 1024;
        rec = (idx_t *)malloc(sizeof(idx_t) * (size_t)cap);
        if (!rec) exit(1);
        rec[cnt++] = 1;
    }
    while (cnt < n) {
        i++;
        v = A241583_pow2mod(v, i);
        if (v > mx) {
            mx = v;
            if (cnt >= cap) {
                cap *= 2;
                idx_t *nr = (idx_t *)realloc(rec, sizeof(idx_t) * (size_t)cap);
                if (!nr) exit(1);
                rec = nr;
            }
            rec[cnt++] = (idx_t)i;
        }
    }
    return (seq_t)rec[n - 1];
}
