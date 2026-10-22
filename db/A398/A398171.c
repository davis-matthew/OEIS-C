/*
 * A398171  a(n) is the number of positive integers that are less than or
 *          equal to and relatively prime to exactly one divisor of n.
 * https://oeis.org/A398171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the number of x (1 ≤ x ≤ n) that are relatively prime
 * to exactly one divisor of n.  The implementation enumerates all divisors
 * of n, counts for each x how many divisors are coprime with x, and
 * returns the number of x with count = 1.  This works in O(d(n)·n)
 * time where d(n) is the divisor count, which is fast enough for n ≤ 10 000.
 * It is written in syntax that compiles both in C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>   /* malloc, realloc, free, calloc, qsort */

/* small gcd for unsigned 64‑bit integers (needed only in C code) */
static inline uint64_t ugcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* comparator for qsort on uint64_t */
static int cmp_uint64(const void *p1, const void *p2) {
    uint64_t a = *(const uint64_t*)p1;
    uint64_t b = *(const uint64_t*)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* external linkage as required */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;            /* offset starts at 1 */

    /* Work with 64‑bit integer for manipulating indices; safe for n ≤ 10 000 */
    uint64_t nn = (uint64_t)n;
    size_t nsize = (size_t)nn;             /* size_t used for array lengths */

    /* ---------- build list of divisors of nn ---------- */
    size_t divcap = 64;
    size_t divcnt = 0;
    uint64_t *divs = (uint64_t*)malloc(divcap * sizeof(uint64_t));
    if (!divs) return (seq_t)0;            /* allocation failure */

    for (uint64_t i = 1; i * i <= nn; ++i) {
        if (nn % i == 0) {
            if (divcnt >= divcap) {
                divcap = divcap * 2;
                uint64_t *tmp = (uint64_t*)realloc(divs, divcap * sizeof(uint64_t));
                if (!tmp) { free(divs); return (seq_t)0; }
                divs = tmp;
            }
            divs[divcnt++] = i;

            if (i * i != nn) {
                if (divcnt >= divcap) {
                    divcap = divcap * 2;
                    uint64_t *tmp = (uint64_t*)realloc(divs, divcap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return (seq_t)0; }
                    divs = tmp;
                }
                divs[divcnt++] = nn / i;
            }
        }
    }

    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

    /* ---------- count for each x how many divisors satisfy gcd(x,d)=1 ---------- */
    uint16_t *cnt = (uint16_t*)calloc(nsize + 1, sizeof(uint16_t));
    if (!cnt) { free(divs); return (seq_t)0; }

    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        for (uint64_t x = 1; x <= d; ++x) {
            if (ugcd(x, d) == 1)
                ++cnt[(size_t)x];          /* safe: ≤ divisor count, fits in uint16_t */
        }
    }

    /* ---------- compute the answer ---------- */
    seq_t result = (seq_t)0;
    for (size_t x = 1; x <= nsize; ++x) {
        if (cnt[x] == 1)
            result = result + (seq_t)1;
    }

    /* ---------- clean up ---------- */
    free(cnt);
    free(divs);
    return result;
}
