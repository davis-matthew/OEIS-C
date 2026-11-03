/*
 * A363187  Prime numbers that are the average of three consecutive odd
 *          semiprimes.
 * https://oeis.org/A363187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 286, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9815 (2026-11-03)
 * Helpers:   build_spf, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime numbers that are the average of three consecutive odd semiprimes */
/* a(n) = the n-th prime p such that there exist odd semiprimes x<y<z with (x+y+z)/3 = p */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
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

#define MAX_CACHE 15000   /* sufficient for n up to ~10000 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 */

    /* cache for sequence terms */
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;

    /* state for scanning odd numbers */
    static idx_t cur = 9;               /* next odd number to test       */
    static idx_t last[3] = {0, 0, 0};   /* last three odd semiprimes      */
    static size_t semiprime_seen = 0;   /* how many odd semiprimes seen  */

    /* Small‑prime‑factor table */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;         /* largest n for which SPF is built */

    /* ensure we have enough terms in the cache */
    while (cache_len < n)
    {
        /* Ensure SPF table covers the current odd number cur */
        if (cur > spf_limit)
        {
            idx_t new_limit = cur * 2;
            if (spf) free(spf);
            build_spf(new_limit, &spf);
            spf_limit = new_limit;
        }

        /* Test whether cur is an odd semiprime */
        if (cur & 1 && spf[cur] < cur)   /* composite and odd */
        {
            idx_t f1 = spf[cur];
            idx_t f2 = cur / f1;
            if (f2 >= f1 && spf[f2] == f2)   /* both factors prime */
            {
                /* shift the circular buffer */
                last[2] = last[1];
                last[1] = last[0];
                last[0] = cur;
                semiprime_seen++;

                if (semiprime_seen >= 3)
                {
                    /* compute the average using seq_t arithmetic */
                    seq_t sum = last[0] + last[1] + last[2];
                    if (sum % 3 == 0)
                    {
                        seq_t avg = sum / 3;
                        if (is_prime_big(avg))
                        {
                            if (cache_len < MAX_CACHE)
                                cache[cache_len++] = avg;
                            else
                                /* escalation not required for supported n range */
                                break;
                        }
                    }
                }
            }
        }

        cur += 2;   /* next odd number */
    }

    return cache[n - 1];
}
