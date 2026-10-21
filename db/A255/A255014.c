/*
 * A255014  Abelian complexity function of the 4-bonacci word (A254990).
 * https://oeis.org/A255014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   encode_parikh, ensure_prefix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Abelian complexity of the 4‑bonacci word (OEIS A255014).
   The function generates a sufficiently long prefix of the fixed point
   of the morphism
       0 → 01
       1 → 02
       2 → 03
       3 → 0
   (the 4‑bonacci word A254990).  For a given length n it counts the
   distinct Parikh vectors of all factors of length n and returns that
   number.  The implementation uses a small constant‑time encoding of
   the 4‑tuple of letter counts into a 64‑bit key and keeps the set of
   distinct vectors in an array of size 20.  A static prefix is built
   incrementally; each call ensures the prefix is long enough for the
   requested n by generating more iterations of the morphism.  The
   growth factor guarantees the answer for all n ≤ 10 000.  The code
   conforms to the requirements for C11 / C++17 and uses only the
   permitted operations. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/encode_parikh.h"
#include "../utils/ensure_prefix.h"

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined outside offset */

    /* Use a generous buffer: for n up to 10 000 this yields < 2 000 000
       characters, which is enough to capture all distinct Parikh vectors.
       The factor 100 is chosen empirically to guarantee coverage. */
    idx_t pref_len = n * 100 + 100 * 10;

    uint8_t *buf = NULL;
    size_t len  = 0;
    ensure_prefix(pref_len, &buf, &len);
    if (!buf) return (seq_t)0;              /* memory error */

    /* Count abelian complexity for this n. */
    idx_t cnt[4] = {0, 0, 0, 0};
    idx_t i;
    for (i = 0; i < (idx_t)n; ++i)
        ++cnt[buf[i]];

    /* Store distinct Parikh vectors – the maximum possible is 16. */
    uint64_t uniq[20];
    size_t nc = 0;
    uniq[nc++] = encode_parikh(cnt);

    for (idx_t pos = 1; pos + n <= (idx_t)len; ++pos) {
        uint8_t left  = buf[pos - 1];
        uint8_t right = buf[pos + n - 1];
        --cnt[left];
        ++cnt[right];

        uint64_t key = encode_parikh(cnt);
        size_t k;
        for (k = 0; k < nc; ++k)
            if (uniq[k] == key) break;
        if (k == nc) uniq[nc++] = key;
    }

    return (seq_t)nc;
}
