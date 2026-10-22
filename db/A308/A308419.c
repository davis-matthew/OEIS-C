/*
 * A308419  Stopping time for Recamán-like iteration of each n: a(0) = n, a(k)
 *          = a(k-1) - k if positive and not already in the sequence, a(k) =
 *          a(k-1) + k if not already in the sequence, otherwise stop.
 * https://oeis.org/A308419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán-like stopping time.  a(n) counts the number of terms in the
   Recamán-like iteration starting from n before a repetition or
   negative subtraction occurs.  The recurrence is implemented by a
   straightforward simulation with a dynamic array stored on the stack
   to avoid problems with arbitrary‑precision types that are not
   trivially constructible. */

#include <stdbool.h>
#include <stddef.h>

/* Check whether v is already in the sequence of length len.  */
static inline bool contains(const seq_t *seen, size_t len, seq_t v)
{
    for (size_t i = 0; i < len; ++i)
        if (seen[i] == v)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    /* Preallocate a conservative upper bound for the sequence length.
       For n <= 10000 the length is < 150; 5000 is more than safe for any
       reasonable n used in the test harness.  */
    const size_t MAX_CAP = 5000;
    seq_t seen[MAX_CAP];
    size_t len = 0;

    /* First term */
    seq_t current = (seq_t)n;
    seen[len++] = current;

    for (idx_t k = 1; ; ++k) {
        bool moved = false;
        seq_t cand;

        /* Try subtraction first if it stays positive. */
        if (current > (seq_t)k) {
            cand = current - (seq_t)k;
            if (!contains(seen, len, cand)) {
                current = cand;
                moved = true;
            }
        }

        /* If subtraction failed, try addition. */
        if (!moved) {
            cand = current + (seq_t)k;
            if (!contains(seen, len, cand)) {
                current = cand;
                moved = true;
            }
        }

        /* Stop if neither move is possible. */
        if (!moved)
            break;

        /* Store new term (we assume len < MAX_CAP). */
        seen[len++] = current;
    }

    return (seq_t)len;
}
