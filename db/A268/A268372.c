/*
 * A268372  Number of distinct primes (n mod k) for k=1..floor(n/2).
 * https://oeis.org/A268372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct primes among { n mod k | k = 1 ..
 *            floor(n/2) }
 * Range:     u8 n <= 825, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation uses a small boolean table to remember which residues
    seen as primes.  For each k in the range 1..floor(n/2) we compute r = n % k.
    If r is prime (checked via the utility function isPrimeTrial) and has not
    been seen before, we increment the distinct‑prime counter.  After each
    call the table is cleared.  The table size is fixed to 5001, which covers
    all possible residues for n <= 10000 (the range in which the build tests
    run).  This keeps the runtime well below the 2‑second limit for the
    intended input sizes.  All arithmetic uses idx_t and seq_t as required,
    and the function keeps external linkage. */

#include <stdbool.h>
#include <string.h>
#include "../utils/isPrimeTrial.h"   /* provides bool isPrimeTrial(seq_t) */

static bool seen[5001];              /* seen[p] == true ⇔ prime p already counted */

seq_t a(idx_t n)
{
    const idx_t half = n / 2;
    seq_t out = 0;

    /* Scan through k = 1 .. floor(n/2) */
    for (idx_t k = 1; k <= half; ++k) {
        const idx_t r = n % k;            /* 0 ≤ r < k ≤ n/2 ≤ 5000 */
        if (isPrimeTrial((seq_t)r)) {      /* r is a prime? */
            if (!seen[(size_t)r]) {        /* not yet counted?  */
                seen[(size_t)r] = true;
                ++out;
            }
        }
    }

    /* Reset table for next call */
    memset(seen, 0, sizeof(seen));

    return out;
}
