/*
 * A322026  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A007814(i) = A007814(j) and A007949(i) = A007949(j), for all i,
 *          j, where A007814 and A007949 give the 2- and 3-adic valuations of
 *          n.
 * https://oeis.org/A322026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9639, u16 n <= 9639, u32 n <= 9639, u64 n <= 9639,
 *            u128 n <= 9639, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = |{ (i,j) ∈ ℕ² : 2^i · 3^j ≤ 2^v2(n) · 3^v3(n) }|
 * where v2(n) (resp. v3(n)) is the exponent of 2 (resp. 3) in n.
 * This is the lexicographically earliest restricted‑growth sequence of
 * the ordered pair [v2(n), v3(n)].
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* ---- compute 2‑adic and 3‑adic valuations ---- */
    unsigned v2 = 0;
    uint64_t x = n;
    while ((x & 1ULL) == 0) { v2++; x >>= 1; }

    unsigned v3 = 0;
    while (x % 3ULL == 0) { v3++; x /= 3ULL; }

    /* ---- bound = 2^v2 · 3^v3 ---- */
    __uint128_t bound128 = ( (__uint128_t)1ULL << v2 );
    for (unsigned i = 0; i < v3; ++i) bound128 *= 3ULL;
    uint64_t bound = (uint64_t)bound128;   /* bound ≤ n, fits in 64‑bit */

    /* ---- pre‑compute powers of 3 up to bound ---- */
    uint64_t pow3[65];
    size_t pow3_len = 1;
    pow3[0] = 1;
    for (size_t i = 1; ; ++i) {
        __uint128_t next = (__uint128_t)pow3[i-1] * 3ULL;
        if (next > bound) break;
        pow3[i] = (uint64_t)next;
        pow3_len = i + 1;
    }

    /* ---- count pairs (i,j) with 2^i*3^j ≤ bound ---- */
    seq_t count = 0;
    for (unsigned i = 0; ; ++i) {
        if (i >= 64) break;               /* safety, bound < 2^64 */
        uint64_t term = 1ULL << i;        /* 2^i */
        if (term > bound) break;

        uint64_t remaining = bound / term;
        size_t jmax = 0;
        while (jmax + 1 < pow3_len && pow3[jmax + 1] <= remaining)
            ++jmax;
        count += (seq_t)(jmax + 1);
    }

    return count;
}
