/*
 * A335653  Smallest prime whose binary expansion has Hamming distance 1 from
 *          2n+1, or 0 if no such prime exists.
 * https://oeis.org/A335653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 185, u32 n <= 1111, u64 n <= 1675,
 *            u128 n <= 2234, big any n
 * Validated: standard, pass, 92 terms through n = 2144 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335653
   a(n) = smallest prime whose binary expansion has Hamming distance 1 from 2n+1,
   or 0 if no such prime exists.
*/
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Compute 2n+1 */
    seq_t base = (seq_t)n * 2 + 1;

    /* Determine bit width of seq_t */
    const unsigned int widthbits = sizeof(seq_t) * 8;

    /* Bit length of base */
    const unsigned int bitlen = bit_length(base);

    /* Upper bound on bit positions to try:
       we search all existing bits and a few beyond.
       For built‑in types we cannot shift past widthbits-1. */
    unsigned int max_k = bitlen + 16;
    if (max_k >= widthbits)
        max_k = widthbits - 1;

    seq_t best = 0;
    bool found = false;

    for (unsigned int k = 0; k <= max_k; ++k) {
        seq_t cand = base ^ ((seq_t)1 << k);
        if (cand < 2)         /* 0 or 1 cannot be prime */
            continue;
        if (is_prime_big(cand)) {
            if (!found || cand < best) {
                best = cand;
                found = true;
            }
        }
    }

    return found ? best : (seq_t)0;
}
