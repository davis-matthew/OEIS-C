/*
 * A340751  Number of partitions of n into 4 parts such that both the smallest
 *          two parts and the largest two parts are relatively prime.
 * https://oeis.org/A340751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 parts such that the *
 *            smallest two parts and the largest two parts are relatively
 *            prime
 * Range:     u8 n <= 43, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   _coprime_range, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/_coprime_range.h"

#define MAX_PF 6  /* maximum distinct prime factors for numbers <= 10000 */

/* Smallest prime factor table, built up to the largest n seen so far */
static idx_t _spf_cache_n = 0;
static int *_spf = NULL;

/* Build smallest prime factor array up to limit n */
static void _build_spf(idx_t n)
{
    if (n <= _spf_cache_n && _spf != NULL)
        return;               /* already built */

    /* free old table */
    free(_spf);
    _spf = (int*)malloc((n + 1) * sizeof(int));
    if (_spf == NULL)
        return;               /* allocation failed – caller will return 0 */

    for (idx_t i = 0; i <= n; ++i)
        _spf[i] = (int)i;     /* spf[i] = i (i is a prime) initially */

    for (idx_t i = 2; (i * i) <= n; ++i)
    {
        if (_spf[i] == (int)i)             /* i is prime */
        {
            for (idx_t j = i * i; j <= n; j += i)
                if (_spf[j] == (int)j)
                    _spf[j] = (int)i;      /* first (smallest) factor */
        }
    }
    _spf[0] = 0;
    _spf[1] = 0;

    _spf_cache_n = n;
}

/* Factoriate rest into distinct primes (stored in pf, count in pc) */
static void _factor_distinct(idx_t rest, int *pf, int *pc)
{
    int last = 0;
    *pc = 0;
    while (rest > 1)
    {
        int p = _spf[(int)rest];
        if (p != last)
        {
            pf[*pc] = p;
            ++(*pc);
            last = p;
        }
        rest /= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* Ensure spf table covers n */
    _build_spf(n);
    if (_spf == NULL)
        return 0;  /* memory allocation failed */

    seq_t total = 0;

    for (idx_t a1 = 1; a1 <= n / 4; ++a1)
    {
        for (idx_t b = a1; b <= (n - a1) / 3; ++b)
        {
            if (gcd((seq_t)a1, (seq_t)b) != (seq_t)1)
                continue;

            idx_t rest = n - a1 - b;
            idx_t upper = rest / 2;
            if (upper < b)
                continue;  /* no admissible c */

            int pf[MAX_PF];
            int pc = 0;
            _factor_distinct(rest, pf, &pc);

            seq_t add = _coprime_range(b, upper, pf, pc);
            total += add;
        }
    }
    return total;
}
