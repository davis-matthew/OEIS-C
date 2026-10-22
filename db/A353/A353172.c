/*
 * A353172  a(n) is the least k > 1 such that Omega(n) = Omega(n mod k), where
 *          Omega = A001222.
 * https://oeis.org/A353172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k > 1 such that Omega(n) = Omega(n mod k) where
 *            Omega is A001222 (total number of prime factors, counted with
 *            multiplicity). Computation is performed by a sieve for prime
 *            factors up to the current index, then by scanning k from 2
 *            upward until the equalities of Omega hold. Results are...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *answers = NULL;      /* answers[0] = a(1) */
    static int   *spf     = NULL;      /* smallest prime factor */
    static seq_t *omega   = NULL;      /* Omega values */
    static idx_t current_max = 0;      /* maximum n for which arrays are built */

    if (n == 0) return 0;            /* undefined for offset 1 – guard */

    /* Expand internal tables if required. */
    if (n > current_max) {
        idx_t new_max = n;
        size_t old_max = current_max;

        /* Allocate and sieve for smallest prime factor */
        int *new_spf = (int *)malloc((new_max + 1) * sizeof(int));
        if (!new_spf) abort();
        for (idx_t i = 0; i <= new_max; ++i) new_spf[i] = 0;

        for (idx_t i = 2; i <= new_max; ++i) {
            if (new_spf[i] == 0) {
                new_spf[i] = (int)i;
                if (i <= new_max / i) {
                    for (idx_t j = i * i; j <= new_max; j += i)
                        if (new_spf[j] == 0) new_spf[j] = (int)i;
                }
            }
        }

        /* Allocate and compute Omega values */
        seq_t *new_omega = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
        if (!new_omega) { free(new_spf); abort(); }
        new_omega[0] = 0;          /* unused */
        new_omega[1] = 0;          /* Omega(1) = 0 */
        for (idx_t i = 2; i <= new_max; ++i)
            new_omega[i] = new_omega[i / new_spf[i]] + (seq_t)1;

        /* Replace old tables */
        free(spf);
        free(omega);
        spf   = new_spf;
        omega = new_omega;
        current_max = new_max;

        /* Resize answer cache */
        seq_t *new_ans = (seq_t *)realloc(answers, new_max * sizeof(seq_t));
        if (!new_ans) abort();
        answers = new_ans;
        for (size_t i = old_max; i < (size_t)new_max; ++i)
            answers[i] = 0;   /* sentinel for “not yet computed” */
    }

    /* Compute a(n) if not already cached. */
    if (answers[(size_t)(n - 1)] == 0) {
        seq_t target = omega[n];
        seq_t n_seq = (seq_t)n;
        seq_t k = (seq_t)2;
        while (1) {
            seq_t rem = n_seq % k;          /* 0 ≤ rem < k ≤ n_seq */
            if (omega[(size_t)rem] == target) {
                answers[(size_t)(n - 1)] = k;
                break;
            }
            ++k;                            /* look for next candidate */
        }
    }

    return answers[(size_t)(n - 1)];
}
