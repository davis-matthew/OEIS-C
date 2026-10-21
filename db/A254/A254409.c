/*
 * A254409  a(n) is the least x such that tau(x) divides (x-1)^n but not
 *          (x-1)^(n-1), for n >= 2.
 * https://oeis.org/A254409
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least x such that τ(x) divides (x‑1)^n but not
 *            (x‑1)^(n‑1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence equals the smallest odd integer congruent to 3 mod 4 whose divisor count is 2^n.  */
/* This implementation uses a recurrence that avoids brute‑force search:   */
/*   a_{a+1} = a_a · p_a² / p_{n‑a‑1}      (indices are 0‑based)              */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------- prime generation (odd primes up to 200 000) ------------------- */
static uint64_t primes_storage[20000];   /* enough to hold first 10 000 odd primes */
static size_t primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    const int LIMIT = 200000;                    /* covers more than 10 000 primes */
    bool *comp = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    memset(comp, 0, (LIMIT + 1) * sizeof(bool));
    for (int i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            if (i >= 3 && primes_cnt < 20000)  /* store only odd primes */
                primes_storage[primes_cnt++] = (uint64_t)i;
            if ((int64_t)i * i <= LIMIT)
                for (int j = i * i; j <= LIMIT; j += i)
                    comp[j] = true;
        }
    }
    free(comp);
    primes_ready = true;
}

/* ------------------------------ main function --------------------------- */
seq_t a(idx_t n)
{
    /* n ≥ 2 is guaranteed by the problem statement, but guard anyway */
    if (n < 2) return 0;

    init_primes();
    if (n > primes_cnt) return 0;           /* safety check */

    /* candidate = product of first n odd primes  (a = 0 case) */
    seq_t candidate = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        candidate = candidate * (seq_t)primes_storage[i];
    }

    /* search for minimal candidate with τ(x)=2^n and x≡3 (mod 4) */
    seq_t best = 0;
    bool best_set = false;
    idx_t a_limit = n / 2;                  /* floor(n/2) */

    for (idx_t a = 0; a <= a_limit; ++a) {
        /* check the modulo 4 condition */
        if ((idx_t)(candidate % 4) == 3) {
            if (!best_set || candidate < best) {
                best = candidate;
                best_set = true;
            }
        }
        if (a == a_limit) break;            /* finished all a values */

        /* compute next candidate:
           candidate_{a+1} = candidate_a * p_a^2 / p_{n-a-1}   (0‑based indices) */
        candidate = candidate * (seq_t)primes_storage[a];
        candidate = candidate * (seq_t)primes_storage[a];
        idx_t denom_idx = n - a - 1;               /* index of prime to divide by */
        candidate = candidate / (seq_t)primes_storage[denom_idx];
    }

    return best;
}
