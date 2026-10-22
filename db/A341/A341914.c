/*
 * A341914  Number of partitions of n into 10 distinct and relatively prime
 *          parts.
 * https://oeis.org/A341914
 *
 * Offset:    55    (a(55) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * P_{<=10}(n/d - 55) where mu is the
 *            Möbius function and P_{<=10}(s) is the number of partitions of s
 *            into parts not exceeding 10. For s<0 the value is 0
 * Range:     u8 n <= 71, u16 n <= 105, u32 n <= 289, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *part_counts = NULL;          /* P_{<=10}(s) for s >= 0   */
static idx_t part_cap = 0;                 /* size of part_counts       */

static int8_t *mu_values = NULL;           /* Möbius values              */
static idx_t mu_cap = 0;                   /* size of mu_values         */

static void ensure_cache(idx_t n)
{
    if (n < 55) return;

    /* ---- partitions into parts <=10 (P_{<=10}) ---- */
    idx_t need_s = n - 55;
    if (part_cap <= need_s) {
        idx_t new_cap = part_cap ?: 64;
        while (new_cap <= need_s) new_cap <<= 1;
        seq_t *new_part = (seq_t*)realloc(part_counts, new_cap * sizeof(seq_t));
        part_counts = new_part;
        part_cap = new_cap;
        for (idx_t i = 0; i < part_cap; ++i) part_counts[i] = 0;
        if (part_cap > 0) part_counts[0] = 1;      /* empty partition        */
        for (idx_t coin = 1; coin <= 10; ++coin) {
            for (idx_t s = coin; s < part_cap; ++s) {
                part_counts[s] += part_counts[s - coin];
            }
        }
    }

    /* ---- Möbius values up to n ---- */
    if (mu_cap <= n) {
        idx_t new_cap = mu_cap ?: 64;
        while (new_cap <= n) new_cap <<= 1;
        int8_t *new_mu = (int8_t*)realloc(mu_values, new_cap * sizeof(int8_t));
        mu_values = new_mu;
        mu_cap = new_cap;

        bool   *is_comp = (bool*)malloc(mu_cap * sizeof(bool));
        idx_t *primes   = (idx_t*)malloc(mu_cap * sizeof(idx_t));
        idx_t   primes_cnt = 0;

        mu_values[0] = 0;
        if (mu_cap > 1) mu_values[1] = 1;
        for (idx_t i = 2; i < mu_cap; ++i) is_comp[i] = false;

        for (idx_t i = 2; i < mu_cap; ++i) {
            if (!is_comp[i]) {
                primes[primes_cnt++] = i;
                mu_values[i] = -1;
            }
            for (idx_t pj = 0; pj < primes_cnt; ++pj) {
                idx_t p   = primes[pj];
                idx_t prod = i * p;
                if (prod >= mu_cap) break;
                is_comp[prod] = true;
                if (i % p == 0) {
                    mu_values[prod] = 0;
                    break;
                } else {
                    mu_values[prod] = -mu_values[i];
                }
            }
        }

        free(is_comp);
        free(primes);
    }
}

seq_t a(idx_t n)
{
    if (n < 55) return 0;

    ensure_cache(n);

    seq_t pos = 0;   /* sum of terms with mu= +1          */
    seq_t neg = 0;   /* sum of terms with mu= -1          */

    idx_t limit = n;
    for (idx_t d = 1; d * d <= limit; ++d) {
        if (limit % d == 0) {
            /* divisor d ------------------------------------------- */
            idx_t k1 = d;
            idx_t p1 = n / k1;
            if (p1 >= 55) {
                idx_t s1 = p1 - 55;
                int8_t m = mu_values[k1];
                if (m == 1)      pos += part_counts[s1];
                else if (m == -1) neg += part_counts[s1];
            }
            /* complementary divisor -------------------------------- */
            idx_t k2 = limit / d;
            if (k2 != k1) {
                idx_t p2 = n / k2;
                if (p2 >= 55) {
                    idx_t s2 = p2 - 55;
                    int8_t m = mu_values[k2];
                    if (m == 1)      pos += part_counts[s2];
                    else if (m == -1) neg += part_counts[s2];
                }
            }
        }
    }

    /* Since the result is non‑negative, simple unsigned subtraction
       is safe because pos >= neg. */
    return pos - neg;
}
