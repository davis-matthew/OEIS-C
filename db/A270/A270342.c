/*
 * A270342  Positive integers n such that the sum of the Pell numbers
 *          A000129(0) + ... + A000129(n-1) is divisible by n.
 * https://oeis.org/A270342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for n such that the sum of Pell numbers P_0 … P_{n-1} is divisible by n.
   The sum is S_{n-1} = (P_n + P_{n-1} - 1)/2.  The function tests for each candidate n
   the condition S_{n-1} % n == 0, keeping the results in a static list so that
   subsequent calls may be answered in O(1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for already generated terms */
    static seq_t *terms = NULL;   /* array of found n */
    static size_t len   = 0;      /* number of terms stored */
    static size_t cap   = 0;      /* capacity of terms array */
    static idx_t next_candidate = 3;   /* next n to test (sequence starts at 3) */

    /* generate terms until we have at least n of them */
    while (len < n)
    {
        idx_t k = next_candidate++;          /* candidate n */

        /* Determine modulus for the test:
           if n is odd: need T_n ≡ 0 (mod n)
           if n is even: need T_n ≡ 0 (mod 2n)  (equivalent to (sum % n)==0)         */
        uint64_t mod_ll = (k & 1ULL) ? k : (2ULL * k);
        seq_t mod = (seq_t)mod_ll;

        /* Compute Pell numbers P_{k-1} and P_k modulo mod */
        seq_t p_prev = 0;                       /* P_0 */
        seq_t p_curr = 1 % mod;                 /* P_1 */

        for (idx_t i = 2; i <= k; ++i)
        {
            seq_t p_next = ((seq_t)2 * p_curr + p_prev) % mod;
            p_prev = p_curr;
            p_curr = p_next;
        }
        /* After loop: p_curr = P_k mod mod, p_prev = P_{k-1} mod mod */

        /* Compute T_k = P_k + P_{k-1} - 1 modulo mod */
        seq_t Tmod = (p_curr + p_prev + mod - 1) % mod;

        bool valid;
        if (k & 1ULL)   /* k odd */
        {
            valid = (Tmod == 0);
        }
        else            /* k even */
        {
            valid = (Tmod % (seq_t)k == 0);
        }

        if (valid)
        {
            /* store the term */
            if (len == cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) { /* handle OOM: aborting */
                    abort();
                }
                terms = tmp;
                cap   = new_cap;
            }
            terms[len++] = (seq_t)k;
        }
    }

    return terms[n - 1];
}
