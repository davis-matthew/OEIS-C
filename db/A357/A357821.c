/*
 * A357821  Denominators of the partial alternating sums of the reciprocals of
 *          the Dedekind psi function (A001615).
 * https://oeis.org/A357821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( Sum_{k=1..n} (-1)^(k+1) / psi(k) )
 * Range:     u8 n <= 13, u16 n <= 36, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* psi(k) = Dedekind ψ function */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ---------- Prime table ---------------------------------------------------- */
static idx_t max_prime_limit = 0;        /* largest n for which primes are known */
static uint64_t *prime_list = NULL;      /* array of primes up to max_prime_limit */
static size_t prime_list_cnt = 0;        /* number of primes in prime_list */

/* Ensure that prime_list contains all primes ≤ limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= max_prime_limit) return;

    /* free any previously allocated prime list */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
        prime_list_cnt = 0;
    }

    /* allocate a boolean array for composite markers */
    size_t sz = (size_t)limit + 1;
    void *tmp = calloc(sz, 1);          /* calloc is used for zero-initialisation */
    if (!tmp) exit(EXIT_FAILURE);
    bool *is_comp = (bool *)tmp;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* count primes to allocate array */
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) prime_list_cnt++;

    /* store primes */
    tmp = malloc(prime_list_cnt * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    prime_list = (uint64_t *)tmp;

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) prime_list[pos++] = (uint64_t)i;

    free(is_comp);
    max_prime_limit = limit;
}

/* ---------- Dedekind ψ function ------------------------------------------- */
static seq_t psi_of(idx_t k)
{
    if (k == 1) return (seq_t)1;

    seq_t psi = (seq_t)k;
    idx_t tmp = k;

    for (size_t i = 0; i < prime_list_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((idx_t)p * (idx_t)p > tmp) break;
        if (tmp % p == 0) {
            psi = psi * ((seq_t)p + 1) / (seq_t)p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {                 /* remaining prime factor > sqrt(k) */
        uint64_t p = tmp;
        psi = psi * ((seq_t)p + 1) / (seq_t)p;
    }
    return psi;
}

/* ---------- Main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, but return 0 safely */

    /* make sure primes are available for factorisation */
    ensure_primes(n);

    seq_t num_abs = 0;        /* absolute value of current numerator   */
    bool num_neg  = false;    /* true ⇔ numerator is negative          */
    seq_t den     = 1;        /* current denominator, always positive   */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t psi   = psi_of(k);
        int term_sign = (k & 1) ? 1 : -1;            /* (+1) at odd k, (-1) at even k */

        /* Align denominators to LCM(den, psi) */
        seq_t g = gcd(den, psi);
        seq_t lcm = (den / g) * psi;            /* LCM as (den/g)*psi            */
        seq_t psi_div_g = psi / g;
        seq_t term_mul   = den / g;            /* absolute numerator of term */

        /* Part from existing sum: numerator_abs * (psi/g)  */
        seq_t part1_abs = num_abs * psi_div_g;
        bool part1_neg  = num_neg;

        /* Part from the new term: den/g with sign = term_sign */
        seq_t part2_abs = term_mul;
        bool part2_neg  = (term_sign < 0);

        /* Sum the two signed numbers */
        seq_t sum_abs;
        bool sum_neg;
        if (part1_neg == part2_neg) {
            sum_abs = part1_abs + part2_abs;
            sum_neg = part1_neg;
        } else {
            if (part1_abs >= part2_abs) {
                sum_abs = part1_abs - part2_abs;
                sum_neg = part1_neg;
            } else {
                sum_abs = part2_abs - part1_abs;
                sum_neg = part2_neg;
            }
        }
        if (sum_abs == 0) {
            num_abs = 0;
            num_neg = false;
        } else {
            /* Reduce the fraction */
            seq_t g2 = gcd(sum_abs, lcm);
            den = lcm / g2;
            num_abs = sum_abs / g2;
            num_neg = (num_abs != 0) && sum_neg;
        }
    }
    return den;   /* denominator of the alternating partial sum */
}
