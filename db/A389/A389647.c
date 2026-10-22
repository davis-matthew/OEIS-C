/*
 * A389647  a(n) is the smallest k > n such that prime(k) contains the digits
 *          of prime(n) as a substring.
 * https://oeis.org/A389647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest k>n such that prime(k) contains prime(n) as
 *            a substring
 * Range:     u8 n <= 27, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime, int_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/get_prime.h"
#include "../utils/int_to_str.h"

static bool contains_substring(uint32_t pat, uint32_t num)
{
    char buf_pat[12];
    char buf_num[12];
    int_to_str(pat, buf_pat);
    int_to_str(num, buf_num);
    return strstr(buf_num, buf_pat) != NULL;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1, safety */

    uint32_t prime_n = get_prime(n);
    idx_t k = n + 1;

    /* Search forward until a matching prime is found */
    for (;; ++k) {
        uint32_t prime_k = get_prime(k);
        if (contains_substring(prime_n, prime_k))
            return (seq_t)k;
    }
}
