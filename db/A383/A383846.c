/*
 * A383846  A version of the Josephus problem: a(n) is the surviving integer
 *          under the eliminate-eliminate-skip version of the elimination
 *          process.
 * https://oeis.org/A383846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: simulate the eliminate‑eliminate‑skip Josephus process
   for a circle of n people.  Starting from the first person, two
   people are eliminated, the next one is skipped and moved to the
   end of the circle.  Repeat until one remains.  The surviving number
   is a(n).  This implementation runs in O(n) time and uses O(n)
   memory, and works with any seq_t type supported by the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, guard */
    if (n == 1) return 1;      /* trivial case */

    seq_t *buf = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!buf) return 0;        /* allocation failure guard */

    for (idx_t i = 0; i < n; ++i)
        buf[i] = (seq_t)(i + 1);

    idx_t head = 0;          /* index of front element */
    idx_t sz   = n;          /* current number of elements */
    const idx_t cap = n;     /* buffer capacity */

    while (sz > 1)
    {
        /* eliminate first person */
        head = (head + 1) % cap;
        sz--;
        if (sz <= 1) break;

        /* eliminate second person */
        head = (head + 1) % cap;
        sz--;
        if (sz <= 1) break;

        /* skip third person: move to back of the buffer */
        seq_t val = buf[head];
        head = (head + 1) % cap;
        sz--;                      /* remove from front */

        idx_t tail = (head + sz) % cap;
        buf[tail] = val;           /* append to back */
        sz++;                      /* restore size */
    }

    seq_t survivor = buf[head];
    free(buf);
    return survivor;
}
