/*
 * A344294  5-smooth but not 3-smooth numbers k such that A056239(k) >=
 *          2*A001222(k).
 * https://oeis.org/A344294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A344294:
   numbers k = 2^a 3^b 5^c with c ≥ 1 and c ≥ a.  
   These are the 5‑smooth integers that contain factor 5 and
   for which the sum of prime indices of the prime factors
   (a + 2b + 3c) is at least twice the number of prime factors
   (2(a+b+c)), i.e. c ≥ a. */

#include <stdlib.h>
#include <stdint.h>

/* Helper structure to keep a 5‑smooth number and its exponents. */
typedef struct {
    seq_t val;
    unsigned a, b, c;
} Number;

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not defined for n = 0 */

    /* Static storage for all 5‑smooth numbers generated so far. */
    static Number *humble   = NULL;   /* array of all 5‑smooth numbers */
    static size_t hum_cap   = 0;
    static size_t hum_len   = 0;

    /* Static storage for the filtered sequence values. */
    static seq_t *seq       = NULL;   /* array of sequence terms */
    static size_t seq_cap   = 0;
    static size_t seq_len   = 0;

    /* Indices for the humble‑number generator. */
    static size_t i2 = 0, i3 = 0, i5 = 0;

    /* Helper to enlarge an array. */
    if (hum_len == 0) { /* first time: initialise */
        hum_cap = 128;
        humble = (Number *)malloc(hum_cap * sizeof(Number));
        if (!humble) return 0;
        humble[0].val = (seq_t)1;
        humble[0].a   = 0;
        humble[0].b   = 0;
        humble[0].c   = 0;
        hum_len = 1;
    }

    /* Grow sequence array as needed. */
    if (seq_cap == 0) { /* first time */
        seq_cap = 128;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0;
    }

    /* Generate terms until we have at least n terms. */
    while (seq_len < n) {
        /* Generate the next candidate 5‑smooth number. */
        seq_t v2 = humble[i2].val * (seq_t)2;
        seq_t v3 = humble[i3].val * (seq_t)3;
        seq_t v5 = humble[i5].val * (seq_t)5;

        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;

        /* Determine exponents for the new number. */
        unsigned new_a, new_b, new_c;
        if (v2 == next) {
            new_a = humble[i2].a + 1;
            new_b = humble[i2].b;
            new_c = humble[i2].c;
        } else if (v3 == next) {
            new_a = humble[i3].a;
            new_b = humble[i3].b + 1;
            new_c = humble[i3].c;
        } else { /* v5 == next */
            new_a = humble[i5].a;
            new_b = humble[i5].b;
            new_c = humble[i5].c + 1;
        }

        /* Append the new humble number. */
        if (hum_len == hum_cap) {
            hum_cap <<= 1;
            Number *tmp = (Number *)realloc(humble, hum_cap * sizeof(Number));
            if (!tmp) { free(humble); free(seq); return 0; }
            humble = tmp;
        }
        humble[hum_len].val = next;
        humble[hum_len].a   = new_a;
        humble[hum_len].b   = new_b;
        humble[hum_len].c   = new_c;
        ++hum_len;

        /* Advance indices that produced this candidate. */
        if (v2 == next) ++i2;
        if (v3 == next) ++i3;
        if (v5 == next) ++i5;

        /* Add to the sequence if it satisfies the condition. */
        if (new_c >= 1 && new_c >= new_a) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!tmp) { free(humble); free(seq); return 0; }
                seq = tmp;
            }
            seq[seq_len++] = next;
        }
    }

    return seq[n - 1];
}
