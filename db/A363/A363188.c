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
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
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
