/*
 * A257720  Down-up reversals: if n is deficient and s(n) is nondeficient,
 *          where s(n) is the sum of proper divisors of n.
 * https://oeis.org/A257720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1384, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, sum_proper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A257720: Down‑up reversals
   a(n) is the nth integer n such that
        n is deficient (σ(n) < 2n) and
        s(n) = σ(n) – n is non‑deficient (σ(s(n)) ≥ 2·s(n))
   where σ(k) denotes the sum of all positive divisors of k. */

/* The build supplies the types 'seq_t' and 'idx_t'.  Do not define them. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Helper to grow a dynamic array of seq_t when an arbitrary‑precision
   integer class may be used. */
#include "../utils/grow_seq.h"
#include "../utils/sum_proper.h"

/* --------------------------------------------------------------------- */
/*  Sequence storage (zero‑based: sequence[0] is a(1))                   */
/* --------------------------------------------------------------------- */
static seq_t *sequence = NULL;   /* array of sequence terms           */
static size_t seq_cap  = 0;      /* allocated capacity of 'sequence'  */
static size_t seq_size = 0;      /* number of terms currently stored */

/* --------------------------------------------------------------------- */
/*  Sum of proper divisors of a number.                                 */
/*  Uses trial division; fine for the magnitude of values encountered   */
/*  by this sequence (≤ a few millions).                                 */

/* --------------------------------------------------------------------- */
/*  Append a new term to the cached sequence, increasing capacity as     */
/*  necessary.                                                          */
/* --------------------------------------------------------------------- */
static void append_value(seq_t val)
{
    /* Ensure space for one more element */
    if (seq_size + 1 > seq_cap)
        grow_seq(&sequence, &seq_cap, seq_size + 1);

    sequence[seq_size++] = val;
}

/* --------------------------------------------------------------------- */
/*  Generate terms until we have at least 'target' terms                 */
/*  (target is a 1‑based index).                                        */
/* --------------------------------------------------------------------- */
static void generate_until(size_t target)
{
    static idx_t last_checked = 0;     /* last candidate examined */

    while (seq_size < target) {
        ++last_checked;                       /* next integer to test */

        seq_t proper = sum_proper(last_checked);   /* σ(n) – n */

        if (proper < (seq_t)last_checked) { /* n is deficient */
            if (proper > 0) {
                /* Convert s(n) to an idx_t for the next test.
                   The sequence never needs s(n) > 2^64‑1 for the first
                   tens of thousands of terms, so this cast is safe. */
                idx_t s = (idx_t)proper;

                seq_t proper_of_s = sum_proper(s);

                if (proper_of_s >= (seq_t)s) {   /* s(n) non‑deficient */
                    append_value((seq_t)last_checked);
                }
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Public function required by the build.                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below offset is irrelevant */
        return (seq_t)0;

    generate_until((size_t)n);

    return sequence[(size_t)n - 1];
}
