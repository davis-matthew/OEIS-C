/*
 * A339093  Seeds producing -1 under the A339091 protocol.
 * https://oeis.org/A339093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search over seeds s = 1, 2, 3, ...: s is "sterile" if the
 *            A339091 protocol, started from a(1) = s (a(n) = cumulative sum
 *            of odd digits so far when n is even, of even digits so far when
 *            n is odd), never produces s again; a(n) is the n-th sterile
 *            seed. Both cumulative sums only grow, so once both e...
 * Range:     u8 n <= 185, u16 n <= 17733, u32 n <= 17733, u64 n <= 17733,
 *            u128 n <= 17733, big any n
 * Validated: standard, pass, 85 terms through n = 9764 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A339093_list = NULL;
static size_t A339093_cnt = 0, A339093_cap = 0;
static uint64_t A339093_next = 1;

static void A339093_add_digits(uint64_t x, uint64_t *O, uint64_t *E)
{
    while (x > 0) {
        uint64_t d = x % 10;
        if (d & 1) *O += d; else *E += d;
        x /= 10;
    }
}

/* true if seed s never reappears (sterile) */
static bool A339093_sterile(uint64_t s)
{
    uint64_t O = 0, E = 0;
    A339093_add_digits(s, &O, &E);
    uint64_t limit = 1000 + 100 * s;
    for (uint64_t i = 2; i < limit; ++i) {
        uint64_t t = (i & 1) ? E : O;
        if (t == s) return false;
        if (O > s && E > s) return true;
        A339093_add_digits(t, &O, &E);
    }
    return true;
}

seq_t a(idx_t n)
{
    while ((idx_t)A339093_cnt < n) {
        uint64_t s = A339093_next++;
        if (A339093_sterile(s)) {
            if (A339093_cnt == A339093_cap) {
                size_t nc = A339093_cap ? 2 * A339093_cap : 1024;
                uint64_t *p = (uint64_t *)realloc(A339093_list, nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                A339093_list = p;
                A339093_cap = nc;
            }
            A339093_list[A339093_cnt++] = s;
        }
    }
    return (seq_t)A339093_list[n - 1];
}
