/*
 * A277907  a(n) = A007310(A277908(n)).
 * https://oeis.org/A277907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes that appear in consecutive prime pairs with
 *            difference 2 or 4, where the first prime of the pair has odd
 *            (1‑based) index. The sequence begins 5, 7, 11, 13, 17, 19, …
 * Range:     u8 n <= 32, u16 n <= 1750, u32 n <= 7096, u64 n <= 7096,
 *            u128 n <= 7096, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The types seq_t and idx_t are provided by the build system. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

/* ---------- Storage for prime numbers ---------- */
static seq_t   *prime_list = NULL;   /* array of primes discovered */
static size_t  prime_cnt  = 0;       /* number of primes stored */
static size_t  prime_cap  = 0;       /* allocated capacity for prime_list */

/* ---------- Storage for the sequence a(n) ---------- */
static seq_t   *seq_list  = NULL;    /* array of sequence terms */
static size_t  seq_len   = 0;        /* number of terms stored */
static size_t  seq_cap   = 0;        /* allocated capacity for seq_list */

/* ---------- Next number to test for primality ---------- */
static seq_t  next_candidate = 2;    /* start with the first prime */

/* --------------------------------------------------------------------
   Ensure prime_list has at least 'needed' slots.  Resize by doubling
   the capacity until it fits.  Use realloc and abort on failure.
   -------------------------------------------------------------------- */
static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 4;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* --------------------------------------------------------------------
   Ensure seq_list has at least 'needed' slots.  Resize by doubling
   the capacity until it fits.  Use realloc and abort on failure.
   -------------------------------------------------------------------- */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_list = tmp;
        seq_cap  = new_cap;
    }
}

/* --------------------------------------------------------------------
   Generate primes incrementally and append qualifying pairs to
   seq_list until it contains at least 'n' terms.  The algorithm
   examines each new prime together with its predecessor; if the
   pair has gap 2 or 4 and the predecessor has odd 1‑based index,
   both primes are added to the sequence.
   -------------------------------------------------------------------- */
static void ensure_seq_up_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* Test primality of the current candidate. */
        if (isPrimeTrial(next_candidate)) {
            /* Store the new prime. */
            ensure_prime_capacity(prime_cnt + 1);
            prime_list[prime_cnt++] = next_candidate;

            /* Check the pair (previous, current). */
            if (prime_cnt >= 2) {
                /* prime_cnt is the 1‑based index of the current prime. */
                if ((prime_cnt & 1ULL) == 0) {   /* previous index is odd */
                    seq_t diff = prime_list[prime_cnt - 1]
                                 - prime_list[prime_cnt - 2];
                    if (diff == (seq_t)2 || diff == (seq_t)4) {
                        ensure_seq_capacity(seq_len + 2);
                        seq_list[seq_len++] = prime_list[prime_cnt - 2];
                        seq_list[seq_len++] = prime_list[prime_cnt - 1];
                    }
                }
            }
        }

        /* Advance the candidate: skip even numbers > 2. */
        if (next_candidate <= 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/* --------------------------------------------------------------------
   Public function required by the harness.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) abort();           /* the sequence is 1-indexed */
    ensure_seq_up_to(n);
    return seq_list[n - 1];
}
