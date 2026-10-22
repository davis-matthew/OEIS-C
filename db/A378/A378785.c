/*
 * A378785  a(1)=1, a(2)=2, then when n is a Fibonacci number F(k+1), the next
 *          F(k) terms are the first F(k) terms increased by a(n) and put in
 *          reverse order.
 * https://oeis.org/A378785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9982, u16 n <= 9982, u32 n <= 9982, u64 n <= 9982,
 *            u128 n <= 9982, big any n
 * Helpers:   ensure_fib_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A378785 */
/* a(1)=1, a(2)=2.
   For n>=3, if n is a Fibonacci number F(k+1) then
     the next F(k) terms are a(1)+a(n),...,a(F(k))+a(n) taken in reverse order. */

#include <stdlib.h>
#include "../utils/ensure_fib_cap.h"

/* Helper: enlarge seq array if needed. */
static void ensure_seq_cap(size_t need, seq_t **seq, size_t *cap)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    *seq = (seq_t *)realloc(*seq, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* State retained between calls. */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_len = 0;          /* number of terms already stored (a(1)..a(seq_len)) */

    static idx_t *fib = NULL;
    static size_t fib_cap = 0;
    static size_t fib_len = 0;         /* number of Fibonacci numbers stored (fib[0] = F1) */

    static idx_t next_to_process = 2;  /* next index to apply the generation rule to */

    /* Initialise on first call. */
    if (seq_len == 0) {
        /* Sequence array. */
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 0;            /* unused, 1‑based indexing */
        seq[1] = 1;
        seq[2] = 2;
        seq_len = 2;

        /* Fibonacci array. */
        fib_cap = 8;
        fib = (idx_t *)malloc(fib_cap * sizeof(idx_t));
        fib[0] = 1;            /* F1 */
        fib[1] = 1;            /* F2 */
        fib_len = 2;
    }

    /* Return already computed term. */
    if (n <= seq_len) return seq[n];

    /* Generate terms up to n. */
    while (next_to_process <= n) {
        idx_t cur = next_to_process;

        /* Ensure Fibonacci array covers cur. */
        while (fib_len <= cur) {
            idx_t next = fib[fib_len-1] + fib[fib_len-2];
            ensure_fib_cap(fib_len + 1, &fib, &fib_cap);
            fib[fib_len++] = next;
        }

        /* Find if cur is a Fibonacci number. */
        size_t i;
        for (i = 0; i < fib_len; ++i) {
            if (fib[i] == cur) break;
        }

        if (i < fib_len && fib[i] == cur) {
            /* cur = F(k+1), where k = i.  f_k = F(k) = fib[i-1]. */
            if (i > 0) {   /* cur >= 2 always, so i >= 2 when generating */
                seq_t inc = seq[cur];      /* a(cur) */
                idx_t f_k = fib[i-1];     /* number of new terms to add */

                /* Append f_k terms: a(1)+inc .. a(f_k)+inc in reverse order. */
                for (idx_t t = f_k; t >= 1; --t) {
                    seq_len++;
                    ensure_seq_cap((size_t)seq_len + 1, &seq, &seq_cap);
                    seq[seq_len] = seq[t] + inc;
                }
            }
        }

        ++next_to_process;
    }

    return seq[n];
}
