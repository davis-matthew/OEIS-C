/*
 * A276440  a(n) = greatest ludic number (A003309) that divides n.
 * https://oeis.org/A276440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest ludic number (A003309) that divides n
 * Range:     u8 n <= 255, u16 n <= 9262, u32 n <= 9262, u64 n <= 9262,
 *            u128 n <= 9262, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* static state for the ludic sieve */
static char *is_ludic = NULL;       /* 1 if i is ludic, 0 otherwise */
static idx_t sieve_limit = 0;       /* maximum n for which sieve is computed */

/* recompute the ludic sieve up to 'limit' */
static void recompute_ludic(idx_t limit)
{
    char *alive = (char *)malloc((size_t)limit + 1);
    if (!alive) return;            /* allocation failure, give up */
    for (idx_t i = 0; i <= limit; ++i) alive[i] = 1;    /* initially all alive */
    alive[0] = 0;                   /* 0 is unused */

    /* iterate over each number starting from 2 */
    for (idx_t x = 2; x <= limit; ++x)
    {
        if (!alive[x]) continue;    /* already removed */
        /* x survives and becomes a ludic number */
        idx_t count = 0;
        for (idx_t y = x + 1; y <= limit; ++y)
        {
            if (!alive[y]) continue;
            ++count;
            if (count % x == 0) alive[y] = 0;  /* remove every x‑th alive number */
        }
    }

    /* build the final ludic flag array */
    char *new_arr = (char *)malloc((size_t)limit + 1);
    if (!new_arr) { free(alive); return; }
    new_arr[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) new_arr[i] = alive[i];

    free(alive);
    if (is_ludic) free(is_ludic);
    is_ludic = new_arr;
    sieve_limit = limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used, safety */
    if (n > sieve_limit) recompute_ludic(n);

    /* prime factorisation of n (trial division) */
    idx_t tmp = n;
    idx_t fac[16];
    idx_t exp[16];
    size_t fc = 0;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            size_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            fac[fc] = p;
            exp[fc] = e;
            ++fc;
        }
    }
    if (tmp > 1)
    {
        fac[fc] = tmp;
        exp[fc] = 1;
        ++fc;
    }

    /* generate all divisors */
    seq_t divs[128];
    size_t divsz = 1;
    divs[0] = 1;
    for (size_t i = 0; i < fc; ++i)
    {
        idx_t prime = fac[i];
        unsigned int e = exp[i];
        size_t cur = divsz;
        seq_t mult = 1;
        for (unsigned int k = 1; k <= e; ++k)
        {
            mult *= (seq_t)prime;
            for (size_t j = 0; j < cur; ++j)
            {
                if (divsz >= 128) break;          /* safety */
                divs[divsz++] = divs[j] * mult;
            }
        }
    }

    /* find the largest divisor that is ludic */
    seq_t best = 1;
    for (size_t i = 0; i < divsz; ++i)
    {
        seq_t d = divs[i];
        if (d <= (seq_t)sieve_limit && is_ludic[ (size_t)d ])
            if (d > best) best = d;
    }
    return best;
}
