/*
 * A306639  Numbers m such that Sum_{d|m} (sigma(d)/tau(d)) is an integer h
 *          where sigma(k) = the sum of the divisors of k (A000203) and tau(k)
 *          = the number of divisors of k (A000005).
 * https://oeis.org/A306639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m for which Sum_{d|m} sigma(d)/tau(d) is
 *            an integer, where sigma(k) is the sum of the divisors of k and
 *            tau(k) is the number of divisors of k
 * Range:     u8 n <= 9, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- helper: Euclidean GCD for seq_t ---------------------------------- */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- small‑prime factor table ------------------------------------------------- */
static idx_t *spf_arr = NULL;          /* SPF up to current limit           */
static idx_t max_precomp = 0;

/* Rebuild SPF table up to new_limit (inclusive) */
static void recompute_spf(idx_t new_limit)
{
    if (new_limit <= max_precomp) return;

    free(spf_arr);
    spf_arr = (idx_t *)malloc((new_limit + 1) * sizeof(idx_t));
    if (!spf_arr) exit(1);

    for (idx_t i = 0; i <= new_limit; ++i)
        spf_arr[i] = 0;

    /* Sieve to compute smallest prime factor for each number */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (spf_arr[i] == 0) {          /* i is prime */
            spf_arr[i] = i;
            if (i <= new_limit / i)
                for (idx_t j = i * i; j <= new_limit; j += i)
                    if (spf_arr[j] == 0) spf_arr[j] = i;
        }
    }
    max_precomp = new_limit;
}

/* --- recursive sum of sigma(d)/tau(d) over all divisors of a number ----------
 *
 * cur_sigma/tau are the products of contributions from already processed
 * prime factors.  num/den hold the current reduced fraction value of the
 * accumulated sum. */
static void rec_div_sum(int depth,
                        const uint64_t *primes,
                        const int *exps,
                        int pc,
                        seq_t cur_sigma,
                        seq_t cur_tau,
                        seq_t *num,
                        seq_t *den)
{
    if (depth == pc) {
        /* Add cur_sigma/cur_tau to the running fraction */
        seq_t new_num = (*num) * cur_tau + cur_sigma * (*den);
        seq_t new_den = (*den) * cur_tau;
        seq_t g = gcd_seq(new_num, new_den);
        *num = new_num / g;
        *den = new_den / g;
        return;
    }

    uint64_t p = primes[depth];
    int emax = exps[depth];
    seq_t p_pow = 1;  /* p^k for current k */

    for (int k = 0; k <= emax; ++k) {
        /* sigma(p^k) = (p^{k+1}-1)/(p-1) */
        seq_t pk1 = p_pow * (seq_t)p;                      /* p^(k+1) */
        seq_t sigma_val = (pk1 - 1) / ((seq_t)p - 1);
        seq_t tau_val = (seq_t)(k + 1);

        rec_div_sum(depth + 1, primes, exps, pc,
                    cur_sigma * sigma_val,
                    cur_tau * tau_val,
                    num, den);

        p_pow *= (seq_t)p;   /* advance to p^(k+1) for next iteration */
    }
}

/* Test whether a given m satisfies the defining property */
static bool is_good(idx_t m)
{
    if (m == 0) return false;
    if (m == 1) return true;   /* 1 always satisfies the condition */

    /* Factor m using the SPF table */
    idx_t tmp = m;
    uint64_t primes[16];
    int exps[16];
    int pc = 0;
    while (tmp > 1) {
        uint64_t p = spf_arr[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[pc] = p;
        exps[pc] = e;
        ++pc;
    }

    /* Accumulate the sum over all divisors via recursion */
    seq_t num = 0, den = 1;
    rec_div_sum(0, primes, exps, pc, (seq_t)1, (seq_t)1, &num, &den);
    return den == 1;
}

/* --- dynamic construction of the sequence --------------------------------- */
static seq_t *terms = NULL;    /* array of known terms */
static size_t terms_cap = 0;
static size_t terms_len = 0;
static idx_t current_m = 1;    /* next integer to test */

/* Append a new term to the dynamic array */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap == 0 ? 256 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* safety guard, although n≥1 as per offset */

    /* Ensure SPF table spans the numbers we will test */
    idx_t future_limit = current_m;
    if (terms_len >= (size_t)n) future_limit = max_precomp;

    while (terms_len < (size_t)n) {
        if (current_m > max_precomp) {
            idx_t new_limit = (max_precomp == 0) ? 1 : max_precomp * 2;
            if (new_limit < current_m) new_limit = current_m; /* avoid overflow */
            recompute_spf(new_limit);
        }

        if (is_good(current_m))
            append_term((seq_t)current_m);

        ++current_m;
    }

    return terms[n - 1];
}
