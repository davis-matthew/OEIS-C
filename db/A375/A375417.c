/*
 * A375417  a(1)=1; thereafter if n is odd we require that a(n) || a(n+1) be
 *          abundant and otherwise deficient. At each step, choose the
 *          smallest possible integer not yet in the sequence.
 * https://oeis.org/A375417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   digits_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375417:
   a(1)=1; for n>1, if n is odd then a(n)||a(n+1) must be abundant,
   otherwise it must be deficient.  At each step choose the smallest
   unused integer.  This function generates the sequence by brute‑force
   search; it keeps a dynamic array of all previously chosen values
   and checks each candidate against that array. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digits_seq_t.h"

/* Helper: concatenate decimal representations of a and b */
static seq_t concat(seq_t a, seq_t b)
{
    idx_t d = digits_seq_t(b);
    seq_t factor = (seq_t)1;
    for (idx_t i = 0; i < d; ++i)
        factor = factor * (seq_t)10;
    return a * factor + b;
}

/* Helper: divisor sum σ(n) (sum of all positive divisors) */
static seq_t sigma_value(seq_t n)
{
    seq_t result = (seq_t)1;
    seq_t m = n;
    for (seq_t p = (seq_t)2; p <= m / p; ++p) {
        if (m % p == (seq_t)0) {
            seq_t term = (seq_t)1;
            seq_t sum  = (seq_t)1;
            while (m % p == (seq_t)0) {
                m /= p;
                term *= p;
                sum += term;
            }
            result *= sum;
        }
    }
    if (m > (seq_t)1)
        result *= ((seq_t)1 + m);
    return result;
}

/* Dynamic array of sequence values */
static seq_t *seq       = NULL;
static size_t seq_cap   = 0;
static idx_t seq_len    = 0;

/* Grow the array when needed, doubling capacity */
static void grow_seq(void)
{
    size_t new_cap = (seq_cap == 0) ? 4 : seq_cap << 1;
    seq_t *new_ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = new_ptr;
    seq_cap = new_cap;
}

/* Ensure the sequence is generated up to index n (1‑based) */
static void ensure_generated(idx_t n)
{
    if (seq == NULL) {
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = (seq_t)1;          /* a(1) = 1 */
        seq_len = 1;
    }

    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];   /* a(current n) */
        seq_t candidate = (seq_t)0;
        bool found = false;

        while (!found) {
            /* Skip if candidate already used */
            bool used = false;
            for (idx_t i = (idx_t)0; i < seq_len; ++i)
                if (seq[i] == candidate) { used = true; break; }
            if (used) { ++candidate; continue; }

            seq_t cat = concat(prev, candidate);
            seq_t sigma_cat = sigma_value(cat);
            seq_t double_cat = cat + cat;
            bool abundant = sigma_cat > double_cat;
            bool deficient = sigma_cat < double_cat;
            bool want_abundant = (seq_len % 2) == 1; /* odd n wants abundant */

            if ((want_abundant && abundant) || (!want_abundant && deficient)) {
                if (seq_len >= seq_cap) grow_seq();
                seq[seq_len] = candidate;
                ++seq_len;
                found = true;
            } else {
                ++candidate;
            }
        }
    }
}

/* The required public function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;
    ensure_generated(n);
    return seq[n - (idx_t)1];
}
