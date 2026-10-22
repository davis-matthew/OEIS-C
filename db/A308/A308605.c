/*
 * A308605  Number of distinct subset sums of the subsets of the set of
 *          divisors of n. Here 0 (as the sum of an empty subset) is included
 *          in the count.
 * https://oeis.org/A308605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of distinct subset sums of the subsets of the set
 *            of divisors of n (OEIS A308605)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code computes the set of divisors of n, then uses a bitset dynamic programming
 * to determine which sums are attainable from subsets of those divisors,
 * finally counting the set bits (including 0). */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* n is 1‑based; handle n == 0 as undefined but fine */
    uint64_t N = (uint64_t)n;

    /* collect divisors */
    uint64_t divisors[256];
    int dcnt = 0;
    uint64_t sumDiv = 0;

    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            divisors[dcnt++] = i;
            sumDiv += i;
            if (i != N / i) {
                divisors[dcnt++] = N / i;
                sumDiv += N / i;
            }
        }
    }

    /* bitset for reachable sums 0..sumDiv */
    size_t bitlen = sumDiv + 1;               /* number of bits needed */
    size_t bytes   = (bitlen + 7) / 8;        /* number of bytes needed */
    unsigned char *bits = (unsigned char*)calloc(bytes, 1);
    if (!bits) return 0;                      /* allocation failure handled lightly */

    /* initially only sum 0 is reachable */
    bits[0] = 1;

    /* dynamic programming over divisors */
    for (int idx = 0; idx < dcnt; ++idx) {
        uint64_t d = divisors[idx];
        /* iterate sums from high to low to avoid reuse within the same step */
        for (long long s = (long long)(sumDiv - d); s >= 0; --s) {
            size_t byteIdx = (size_t)(s >> 3);
            int    bitPos  = (int)(s & 7);
            if (bits[byteIdx] & (1U << bitPos)) {
                size_t t = (size_t)(s + d);
                bits[t >> 3] |= (1U << (t & 7));
            }
        }
    }

    /* count reachable sums (number of set bits) */
    seq_t count = 0;
    for (size_t s = 0; s < bitlen; ++s) {
        if (bits[s >> 3] & (1U << (s & 7))) {
            ++count;
        }
    }

    free(bits);
    return count;
}
