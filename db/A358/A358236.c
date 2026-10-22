/*
 * A358236  Number of factorizations of n where the sum of the factors is
 *          carryfree when the addition is done in the primorial base.
 * https://oeis.org/A358236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9522, u16 n <= 9522, u32 n <= 9522, u64 n <= 9522,
 *            u128 n <= 9522, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A358236
   Number of unordered factorizations of n whose factors’ sum is carry‑free
   when added in primorial base.  The implementation enumerates all
   multiplicative partitions of n and checks the carry‑free condition
   digitwise. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve (up to n).  Allocates a true array of size n+1 and
   returns the list of primes and its count.  The caller must free the
   array returned in *primes_out.  This routine is safe in both C11
   and C++17 because malloc/calloc return void* and are cast explicitly. */
/* ------------------------------------------------------------------ */
static void gen_primes_upto(uint64_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    bool *is_prime = (bool*)calloc((size_t)(limit + 1u), sizeof(bool));
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (uint64_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (!is_prime[p]) continue;
        for (uint64_t j = p * p; j <= limit; j += p)
            is_prime[j] = false;
    }

    // Count primes
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *pr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!pr) {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            pr[idx++] = i;

    free(is_prime);
    *primes_out = pr;
    *cnt_out = cnt;
}

/* ------------------------------------------------------------------ */
/* Recursive enumeration of factorisations of rem.
   Parameters:
     rem          - remaining product to factor
     min_f        - minimal allowed factor (ensures non‑decreasing order)
     primes       - array of primes for primorial base
     k            - number of digit positions (first k primes)
     sum_digits   - array of running digit sums (size >= k)
     cnt          - pointer to result counter (incremented for each
                    valid factorisation) */
/* ------------------------------------------------------------------ */
static void dfs(uint64_t rem, uint64_t min_f,
                const uint64_t *primes, size_t k,
                seq_t *sum_digits, seq_t *cnt)
{
    if (rem == 1) {
        /* All factors accounted for; the growth check was enforced
           during addition, so this branch is valid. */
        ++(*cnt);
        return;
    }

    for (uint64_t f = min_f; f <= rem; ++f) {
        if (rem % f != 0) continue;          // f is not a divisor

        /* Add f’s digits to the running sum, checking for overflow. */
        bool overflow = false;
        uint64_t temp = f;
        for (size_t i = 0; i < k; ++i) {
            seq_t d = (seq_t)(temp % primes[i]);  // digit at position i
            sum_digits[i] += d;
            if (sum_digits[i] >= (seq_t)primes[i]) overflow = true;
            temp /= primes[i];
            if (temp == 0) break;
        }

        if (!overflow) {
            dfs(rem / f, f, primes, k, sum_digits, cnt);
        }

        /* Undo the addition (subtract the digits). */
        temp = f;
        for (size_t i = 0; i < k; ++i) {
            seq_t d = (seq_t)(temp % primes[i]);
            sum_digits[i] -= d;
            temp /= primes[i];
            if (temp == 0) break;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 by definition */

    /* -------------------------------------------------------------- */
    /* Prime list up to n (cached between calls).                    */
    /* -------------------------------------------------------------- */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t max_n = 0;

    if ((idx_t)n > max_n) {
        if (primes) {
            free(primes);
            primes = NULL;
        }
        gen_primes_upto((uint64_t)n, &primes, &prime_cnt);
        max_n = (idx_t)n;
    }

    /* -------------------------------------------------------------- */
    /* Determine number of primorial digit positions (k).             */
    /* -------------------------------------------------------------- */
    size_t k = 0;
    uint64_t prod = 1;
    while (k < prime_cnt && prod <= (uint64_t)n) {
        prod *= primes[k];
        ++k;
    }
    if (k > 0 && prod > (uint64_t)n) --k;   /* adjust for over‑multiplication */

    /* -------------------------------------------------------------- */
    /* Prepare arrays and count.                                      */
    /* -------------------------------------------------------------- */
    seq_t sum_digits[32];          /* k will never exceed 32 for n <= 10000 */
    for (size_t i = 0; i < k; ++i) sum_digits[i] = 0;
    seq_t result = 0;

    dfs((uint64_t)n, 2, primes, k, sum_digits, &result);
    return result;
}
