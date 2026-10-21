/*
 * A261493  Palindromes that are concatenation of palindromic prime numbers in
 *          increasing order up to the n-th and then in decreasing order.
 * https://oeis.org/A261493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   digit_count, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes palindromic primes up to n and concatenates them in the
   required order: first increasing, then decreasing, excluding the n‑th
   prime from the descending part. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1, but return 0 */

    uint64_t *pr = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!pr)
        return 0; /* allocation failure */

    size_t cnt = 0;

    /* 1‑digit palindromic primes */
    for (uint64_t d = 2; d <= 9 && cnt < n; ++d) {
        if (is_prime(d))
            pr[cnt++] = d;
    }

    /* 2‑digit palindromic prime 11 */
    if (cnt < n) {
        pr[cnt++] = 11;
    }

    /* odd‑length palindromic numbers (len = 3,5,…) */
    for (int len = 3; cnt < n; len += 2) {
        int half = (len + 1) / 2;
        uint64_t start = 1;
        for (int i = 1; i < half; ++i) start *= 10;
        uint64_t end = start * 10;
        for (uint64_t pref = start; pref < end && cnt < n; ++pref) {
            uint64_t pal = pref;
            uint64_t t = pref / 10;
            while (t > 0) {
                pal = pal * 10 + t % 10;
                t /= 10;
            }
            if (is_prime(pal))
                pr[cnt++] = pal;
        }
    }

    /* concatenate */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t pow10d = pow10((idx_t)digit_count(pr[i]));
        result = result * pow10d + p;
    }
    if (cnt > 1) {
        for (long long j = (long long)(cnt - 2); j >= 0; --j) {
            idx_t i = (idx_t)j;
            seq_t p = (seq_t)pr[i];
            seq_t pow10d = pow10((idx_t)digit_count(pr[i]));
            result = result * pow10d + p;
        }
    }

    free(pr);
    return result;
}
