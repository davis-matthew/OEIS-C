/*
 * A363188  Prime numbers that are the exact average of four consecutive odd
 *          semiprimes.
 * https://oeis.org/A363188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   prime equals average of four consecutive odd semiprimes
 * Range:     u8 n <= 1, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* Build smallest prime factor array up to limit. */
static void build_spf(idx_t limit, idx_t *spf)
{
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        if (spf[p] == 0) {
            spf[p] = p;
            if (p <= limit / p) {
                for (idx_t m = p * p; m <= limit; m += p)
                    if (spf[m] == 0) spf[m] = p;
            }
        }
    }
}

static bool is_semiprime(idx_t x, const idx_t *spf)
{
    if (x < 9) return false;                /* smallest odd semiprime is 9 */
    idx_t p = spf[x];
    if (p <= 1 || p == x) return false;    /* not composite or prime */
    idx_t q = x / p;
    if ((q & 1) == 0) return false;         /* q must be odd */
    return (q == spf[q]);                   /* q is prime */
}

seq_t a(idx_t n)
{
    /* initial sieve limit – grows as needed */
    idx_t limit = 2000;
    idx_t *spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) abort();
    build_spf(limit, spf);

    idx_t win[4];
    size_t win_size = 0;
    seq_t win_sum = 0;
    idx_t found = 0;
    seq_t result = 0;

    idx_t cur = 3;  /* start from the first odd number > 1 */

    while (found < n) {
        if (cur > limit) {
            idx_t new_limit = limit * 2;
            idx_t *new_spf = (idx_t *)malloc((new_limit + 1) * sizeof(idx_t));
            if (!new_spf) abort();
            build_spf(new_limit, new_spf);
            free(spf);
            spf = new_spf;
            limit = new_limit;
        }

        if ((cur & 1) && is_semiprime(cur, spf)) {
            win_sum += (seq_t)cur;
            if (win_size < 4) {
                win[win_size++] = cur;
            } else {
                win_sum -= (seq_t)win[0];
                memmove(win, win + 1, 3 * sizeof(idx_t));
                win[3] = cur;
            }

            if (win_size == 4 && (win_sum % 4) == 0) {
                seq_t avg = win_sum / 4;
                if (is_prime_big(avg)) {
                    ++found;
                    if (found == n) {
                        result = avg;
                        break;
                    }
                }
            }
        }

        ++cur;
    }

    free(spf);
    return result;
}
