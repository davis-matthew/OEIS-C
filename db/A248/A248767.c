/*
 * A248767  Greatest 5th power integer that divides n!.
 * https://oeis.org/A248767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{p≤n} p^{5·⌊v_p(n!)/5⌋} (the greatest fifth‑power
 *            integer dividing n!)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 23, u64 n <= 32, u128 n <= 54,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow_seq.h"   /* provides seq_t pow_seq(seq_t, int) */

/* ---------- prime sieve (cached between calls) ---------- */
static uint32_t *prime_list = NULL;  /* array of primes */
static idx_t    prime_cnt  = 0;     /* number of primes stored   */
static idx_t    prime_limit = 0;    /* largest n sieved so far    */

/* Ensure that the primes up to n are available.  */
static void ensure_primes_up_to(idx_t n)
{
    if (n <= prime_limit) return;          /* already sieved */

    /* Allocate a boolean sieve: 0 … n. */
    size_t size = (size_t)n + 1;
    bool *is_prime = (bool *)calloc(size, sizeof(bool));
    if (!is_prime) return;                 /* allocation failure → abort */

    for (idx_t i = 2; i <= n; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= n; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = false;

    /* Count primes and store them. */
    idx_t new_cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i]) ++new_cnt;

    uint32_t *new_list = (uint32_t *)realloc(prime_list, new_cnt * sizeof(uint32_t));
    if (!new_list) { free(is_prime); return; }  /* allocation failure */
    prime_list = new_list;

    idx_t pos = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i]) prime_list[pos++] = (uint32_t)i;

    prime_cnt   = new_cnt;
    prime_limit = n;
    free(is_prime);
}

/* ---------- sequence definition ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;    /* undefined below offset */

    ensure_primes_up_to(n);

    seq_t answer = (seq_t)1;
    for (idx_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if (p > (uint32_t)n) break;

        /* Exponent of p in n!  (Legendre’s formula). */
        idx_t exp_total = 0;
        idx_t tmp = n;
        while (tmp) {
            tmp /= p;
            exp_total += tmp;
        }

        /* Greatest multiple of 5 not exceeding exp_total. */
        idx_t exp_power = exp_total - (exp_total % 5);
        if (exp_power == 0) continue;     /* contributes nothing */

        /* Compute p^{exp_power}.  exp_power is a multiple of 5, so
           exp_power = 5 * floor(v_p(n!)/5).  Use pow_seq for the power. */
        seq_t term = pow_seq((seq_t)p, (int)exp_power);
        answer = answer * term;
    }

    return answer;
}
