/*
 * A368572  Number of refactorable numbers less than n that do not divide n.
 * https://oeis.org/A368572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of refactorable numbers k < n that do not divide
 *            n. A refactorable number k satisfies τ(k) | k, where τ(k) is the
 *            divisor count. The implementation keeps a simple sieve of τ(k)
 *            and a flag array for refactorable numbers. For each call it uses
 *            the stored prefix sums and subtracts the...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static storage */
static size_t cap = 0;           /* maximum n that has been pre‑computed */
static bool *refarr = NULL;      /* refarr[i] == true if i is refactorable */
static uint16_t *tauarr = NULL;  /* τ(i) */
static seq_t *prefarr = NULL;    /* prefix sums of refarr */

/* Ensure pre‑computation up to at least n (exclusive) */
static void ensure_capacity(idx_t n)
{
    if (n <= cap)
        return;                      /* already ready */

    size_t new_cap = (cap == 0) ? 1 : cap;
    while (new_cap < n)
        new_cap <<= 1;                /* double until big enough */

    /* Allocate fresh arrays */
    bool *new_ref = (bool*)malloc((new_cap + 1) * sizeof(bool));
    uint16_t *new_tau = (uint16_t*)malloc((new_cap + 1) * sizeof(uint16_t));
    seq_t *new_pref = (seq_t*)malloc((new_cap + 1) * sizeof(seq_t));

    if (!new_ref || !new_tau || !new_pref) {
        /* memory exhaustion: abort safely */
        abort();
    }

    /* Zero initialisation of τ and prefix arrays */
    for (size_t i = 0; i <= new_cap; ++i) {
        new_tau[i] = 0;
        new_pref[i] = (seq_t)0;
    }

    /* Compute divisor counts τ(i) */
    for (size_t i = 1; i <= new_cap; ++i) {
        for (size_t j = i; j <= new_cap; j += i) {
            new_tau[j]++;               /* one more divisor of j */
        }
    }

    /* Determine refactorable flags and prefix sums */
    new_pref[0] = (seq_t)0;
    for (size_t i = 1; i <= new_cap; ++i) {
        bool ref = (new_tau[i] != 0 && (i % new_tau[i]) == 0);
        new_ref[i] = ref;
        new_pref[i] = new_pref[i - 1] + (ref ? (seq_t)1 : (seq_t)0);
    }

    /* Release old storage */
    free(refarr);
    free(tauarr);
    free(prefarr);

    /* Install new storage */
    refarr = new_ref;
    tauarr = new_tau;
    prefarr = new_pref;
    cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 0 by definition */
        return (seq_t)0;

    /* Pre‑compute tables up to n-1 */
    ensure_capacity(n - 1);

    /* Count of all refactorable numbers < n */
    seq_t result = prefarr[n - 1];

    /* Subtract those that also divide n (excluding n itself) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t d1 = d;
        idx_t d2 = n / d;

        if (d1 < n && refarr[d1])
            result -= (seq_t)1;

        if (d2 != d1 && d2 < n && refarr[d2])
            result -= (seq_t)1;
    }

    return result;
}
