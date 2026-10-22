/*
 * A371895  a(n) is the least k>0 such that n*k contains the n-th prime as a
 *          substring.
 * https://oeis.org/A371895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = least k > 0 such that decimal representation of (n*k)
 *            contains the decimal representation of the n-th prime as a
 *            substring. The sequence is computed by brute‑force searching for
 *            k
 * Range:     u8 n <= 9, u16 n <= 320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/get_prime.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n) {
    /* Get the n-th prime as a string */
    uint32_t pn = get_prime(n);          /* n ≥ 1 */
    char prime_buf[64];
    size_t prime_len = 0;
    seq_to_str((seq_t)pn, prime_buf, &prime_len);
    prime_buf[prime_len] = '\0';

    /* Buffer for product string */
    char prod_buf[128];
    size_t prod_len;
    seq_t k = 1;

    for (;;) {
        seq_t product = (seq_t)n * k;
        seq_to_str(product, prod_buf, &prod_len);
        prod_buf[prod_len] = '\0';

        /* substring search */
        if (strstr(prod_buf, prime_buf) != NULL)
            return k;
        ++k;
    }
}
