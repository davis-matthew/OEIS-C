/*
 * A248034  a(n+1) gives the number of occurrences of the last digit of a(n)
 *          so far, up to and including a(n), with a(0)=0.
 * https://oeis.org/A248034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, hear
 * Formula:   not stated
 * Range:     u8 n <= 610, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0;
   For n>=0, a(n+1) = number of occurrences of the last decimal digit of a(n)
   in the sequence a(0),...,a(n). */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static storage for the sequence and the per‑digit counts. */
    static seq_t *seq   = NULL;      /* array of terms */
    static size_t seq_len = 0;       /* number of terms currently stored */
    static size_t seq_cap = 0;       /* capacity of the allocated array */
    static unsigned long long cnt[10] = { 0 };  /* count of each digit 0‑9 */

    /* Initialise on first call. */
    if (seq == NULL) {
        seq_cap = 256;                 /* initial capacity; grows geometrically */
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq == NULL) { exit(1); }  /* OOM; abort */
        seq[0] = (seq_t)0;             /* a(0) = 0 */
        seq_len = 1;
        cnt[0] = 1;                    /* digit '0' has appeared once (from a(0)) */
    }

    /* Grow the sequence until it contains a(n). */
    while (seq_len <= (size_t)n) {
        /* Ensure enough capacity. */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (tmp == NULL) { exit(1); }  /* OOM; abort */
            seq = tmp;
            seq_cap = new_cap;
        }

        /* Current term and its last decimal digit. */
        seq_t cur = seq[seq_len - 1];
        seq_t last_digit = cur % (seq_t)10;

        /* Next term is the count of that digit so far. */
        seq_t next = (seq_t)cnt[(unsigned)last_digit];
        seq[seq_len] = next;

        /* Update the per‑digit counts using the digits of the new term. */
        seq_t x = next;
        if (x == 0) {
            cnt[0]++;                  /* single digit '0' */
        } else {
            while (x > 0) {
                unsigned d = (unsigned)(x % (seq_t)10);
                cnt[d]++;
                x = x / (seq_t)10;
            }
        }

        seq_len++;
    }

    return seq[(size_t)n];
}
