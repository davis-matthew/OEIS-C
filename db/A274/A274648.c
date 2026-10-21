/*
 * A274648  A variation on Recamán's sequence (A005132): a(n) is the first
 *          positive number of the form a(n-1)-n*k, k>0 not already in the
 *          sequence; and if no such number exists, then a(n) is the first
 *          number of the form a(n-1)+n*k, k>0 not already in the sequence.
 * https://oeis.org/A274648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán variant (OEIS A274648)
   a(0) = 0
   For n > 0 let p = a(n‑1).
   1. Try values p − n·k, k = 1,2,… in increasing order until
      the first positive value not already in the sequence is found.
   2. If no such value exists, try values p + n·k, k = 1,2,… until
      the first value not already in the sequence is found.  */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Dynamic array storing the sequence and a helper to check membership   */
static seq_t *Seq = NULL;      /* array of computed terms   */
static idx_t SeqLen = 0;       /* number of terms computed */
static size_t SeqCap = 0;      /* capacity of Seq           */

/* Ensure capacity for at least needed+1 elements (needed is an index) */
static void seq_ensure(idx_t needed)
{
    if (SeqCap <= (size_t)needed) {
        size_t new_cap = SeqCap ? SeqCap * 2 : 16;
        while (new_cap <= (size_t)needed) new_cap <<= 1;
        Seq = (seq_t *)realloc(Seq, new_cap * sizeof(seq_t));
        SeqCap = new_cap;
    }
}

/* Linear‑search membership test in the already computed prefix of Seq */
static bool seq_contains(seq_t value, idx_t upto)
{
    for (idx_t i = 0; i < upto; ++i)
        if (Seq[i] == value)
            return true;
    return false;
}

/* Compute terms up to index n (inclusive) */
static void seq_compute_to(idx_t n)
{
    if (SeqLen == 0) {          /* initialise a(0) */
        seq_ensure(0);
        Seq[0] = (seq_t)0;
        SeqLen = 1;
    }

    for (idx_t i = SeqLen; i <= n; ++i) {
        seq_ensure(i);
        seq_t prev = Seq[i - 1];
        idx_t step = i;              /* n in the definition */
        seq_t result = 0;
        bool found = false;

        /* --- try decreasing candidates p − step·k --- */
        for (idx_t k = 1; ; ++k) {
            seq_t prod = (seq_t)step * k;   /* product fits in seq_t safely */
            if (prod > prev)                /* would become non‑positive */
                break;
            seq_t cand = prev - prod;
            if (!seq_contains(cand, i)) {
                result = cand;
                found = true;
                break;
            }
        }

        /* --- if none, try increasing candidates p + step·k --- */
        if (!found) {
            idx_t k = 1;
            while (true) {
                seq_t prod = (seq_t)step * k;
                seq_t cand = prev + prod;
                if (!seq_contains(cand, i)) {
                    result = cand;
                    break;
                }
                ++k;
            }
        }

        Seq[i] = result;
        SeqLen = i + 1;
    }
}

/* --------------------------------------------------------------------- */
/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n >= SeqLen)
        seq_compute_to(n);
    return Seq[(size_t)n];
}
