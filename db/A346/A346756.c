/*
 * A346756  Lesser emirps (A109308) subtracted from their reversals.
 * https://oeis.org/A346756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A346756:  a(n) = reverse(A109308(n)) − A109308(n)
    Where A109308(n) is the n‑th lesser emirp: a prime p with
    reverse(p) also prime and p < reverse(p). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*--- Reverse a decimal integer ------------------------------*/
static idx_t reverse_uint(idx_t v)
{
    idx_t r = 0;
    while (v > 0) {
        r = r * 10 + v % 10;
        v /= 10;
    }
    return r;
}

/*--- Sieve of Eratosthenes ---------------------------------*/
static unsigned char *prime_flag = NULL;   /* 1 = prime, 0 = composite */
static const idx_t SIEVE_LIMIT = 10000000ULL;   /* >= 10 000th lesser emirp */

/* Build the sieve once.                                          */
static void init_sieve(void)
{
    if (prime_flag) return;               /* already done */

    prime_flag = (unsigned char *)calloc(SIEVE_LIMIT + 1, sizeof(unsigned char));
    if (!prime_flag) return;              /* out of memory – let caller fail */

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) prime_flag[i] = 1;

    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!prime_flag[i]) continue;
        for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
            prime_flag[j] = 0;
    }
}

/*--- Dynamic array for already computed differences ---------*/
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/*--- Remember the last prime we checked ----------------------*/
static idx_t next_prime_start = 2;

/*--- Main sequence function ----------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; offset is 1 */

    /* Build sieve once */
    if (!prime_flag) init_sieve();

    /* Produce emirps until we have at least n of them */
    while (seq_len < n) {
        for (idx_t p = next_prime_start; p <= SIEVE_LIMIT; ++p) {
            if (!prime_flag[p]) continue;
            idx_t rev = reverse_uint(p);
            if (rev <= p) continue;             /* require p < rev(p) */
            if (rev > SIEVE_LIMIT) continue;     /* reverse unknown – skip */
            if (!prime_flag[rev]) continue;      /* reverse must be prime */

            /* p is a lesser emirp */
            seq_t diff = (seq_t)rev - (seq_t)p;
            ensure_seq_capacity(seq_len + 1);
            seq_arr[seq_len++] = diff;
            next_prime_start = p + 1;
            break;                       /* found next emirp, go back to while */
        }

        /* In normal circumstances the loop above always finds a new emirp
           before reaching the sieve limit.  For n <= 10 000 the SIEVE_LIMIT
           of 10 000 000 is sufficient, so we need not extend it. */
    }

    return seq_arr[n - 1];
}
