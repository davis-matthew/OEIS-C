/*
 * A377784  First-differences of A377783 (least nonsquarefree number >
 *          prime(n)).
 * https://oeis.org/A377784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A377783(p_{n+1}) - A377783(p_n) where p_k is the k-th
 *            prime and A377783(p) = minimal x > p that is not square‑free
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Prime generation – enough for n up to about 10000 (≤ 104729)       */
/* ------------------------------------------------------------------ */
#define FULL_PRIME_LIMIT 200000          /* > 10 000th prime (≈104 729)  */
#define SMALL_PRIME_LIMIT 2000           /* primes needed for A(p)      */

static idx_t full_primes[20000];         /* prime list – static capacity */
static size_t full_prime_cnt = 0;
static idx_t small_primes[500];          /* small primes – static capacity */
static size_t small_prime_cnt = 0;

/* Initialization once, thread‑safe for static data */
static void init_primes(void)
{
    if (full_prime_cnt > 0) return;       /* already initialized */

    bool *is_comp = (bool *)calloc(FULL_PRIME_LIMIT + 1, sizeof(bool));
    if (is_comp == NULL) abort();        /* out of memory */

    for (idx_t i = 2; i <= FULL_PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (full_prime_cnt < sizeof(full_primes)/sizeof(full_primes[0]))
                full_primes[full_prime_cnt++] = i;
            else abort();                /* capacity exceeded */

            if (i <= SMALL_PRIME_LIMIT) {
                if (small_prime_cnt < sizeof(small_primes)/sizeof(small_primes[0]))
                    small_primes[small_prime_cnt++] = i;
                else abort();
            }

            if (i * i <= FULL_PRIME_LIMIT) {
                for (idx_t j = i * i; j <= FULL_PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }

    free(is_comp);
}

/* ------------------------------------------------------------------ */
/* Compute A377783(p): smallest nonsquare‑free integer > prime p     */
/* ------------------------------------------------------------------ */
static idx_t A_of_prime(idx_t p)
{
    idx_t best = 0;
    for (size_t k = 0; k < small_prime_cnt; ++k) {
        idx_t q = small_primes[k];
        idx_t sq = q * q;                     /* fits in idx_t       */
        /* Ceiling of (p+1) / sq, times sq   */
        idx_t mult = (p + 1 + sq - 1) / sq;
        idx_t candidate = mult * sq;
        if (best == 0 || candidate < best)
            best = candidate;
    }
    return best;                                /* always > p           */
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* undefined position   */

    init_primes();                             /* ensure prime tables  */

    /* 0‑based index: prime p_n is full_primes[n‑1]          */
    if (n + 1 > full_prime_cnt)                 /* safety check          */
        abort();

    idx_t p_n   = full_primes[n - 1];
    idx_t p_n1  = full_primes[n];               /* next prime            */

    idx_t A_pn   = A_of_prime(p_n);
    idx_t A_pn1  = A_of_prime(p_n1);

    idx_t diff = A_pn1 - A_pn;
    return (seq_t)diff;
}
