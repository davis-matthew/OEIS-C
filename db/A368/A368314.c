/*
 * A368314  a(n) is the number of numbers that can be obtained by replacing
 *          any positive number without leading zeros, say m, appearing in the
 *          binary expansion of n by one of the divisors of m.
 * https://oeis.org/A368314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts the distinct numbers that can be obtained by replacing any
   binary substring m of the binary expansion of n with one of the divisors
   of m (including m itself).  Example: for n = 42 the set is
   {1, 2, 3, 5, 6, 7, 10, 14, 21, 22, 26, 42}.  Thus a(42) = 12. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* compare function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* helper: bit length of a 64‑bit integer (assumes x > 0) */
static inline uint64_t bitlen_uint64(uint64_t x)
{
    return 64 - __builtin_clzll(x);
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for the given offset, safe fallback */

    /* bit length of n */
    uint64_t L = bitlen_uint64((uint64_t)n);   /* 1 ≤ L ≤ 64 */

    /* storage for the candidate new numbers (max ≈ 8000 for n ≤ 10000) */
    uint64_t arr[8192];
    size_t cnt = 0;

    /* enumerate all substrings */
    for (uint64_t i = 0; i < L; ++i) {
        /* skip substrings that start with 0 (no leading zeros allowed) */
        if (!((n >> (L - 1 - i)) & 1ULL))
            continue;

        for (uint64_t j = i; j < L; ++j) {
            uint64_t l = j - i + 1;

            /* value of the substring m */
            uint64_t mask = (l == 64) ? ~0ULL : ((1ULL << l) - 1ULL);
            uint64_t m = (n >> (L - 1 - j)) & mask;

            /* suffix of the original number after j */
            uint64_t suffix_len = L - j - 1;
            uint64_t suffix = (suffix_len == 0) ? 0ULL
                                                : (n & ((1ULL << suffix_len) - 1ULL));

            /* prefix before i */
            uint64_t prefix = n >> (L - i);

            /* enumerate divisors of m */
            for (uint64_t d = 1; d * d <= m; ++d) {
                if (m % d != 0)
                    continue;

                /* replace with divisor d */
                uint64_t len_d = bitlen_uint64(d);
                uint64_t new_val = (prefix << (len_d + suffix_len)) | (d << suffix_len) | suffix;
                arr[cnt++] = new_val;

                /* distinct divisor m/d */
                uint64_t d2 = m / d;
                if (d2 != d) {
                    uint64_t len_d2 = bitlen_uint64(d2);
                    uint64_t new_val2 = (prefix << (len_d2 + suffix_len)) | (d2 << suffix_len) | suffix;
                    arr[cnt++] = new_val2;
                }
            }
        }
    }

    /* sort the array and count unique values */
    qsort(arr, cnt, sizeof(uint64_t), cmp_uint64);
    size_t unique = 1;          /* at least the original number itself (always present) */
    for (size_t k = 1; k < cnt; ++k)
        if (arr[k] != arr[k - 1])
            ++unique;

    return (seq_t)unique;
}
