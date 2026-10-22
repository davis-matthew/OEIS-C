/*
 * A318928  Runs-resistance of binary representation of n.
 * https://oeis.org/A318928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A318928 – runs‑resistance of the binary
   representation of n.  The value is the number of times the RUNS
   transformation must be applied to the list of bits of n in order
   to reduce it to a single element.  (See a(1)=1, a(2)=2, etc.) */
seq_t a(idx_t n)
{
    /* buffer for bits of n (max 64 bits) – stored least‑significant
       bit first; each element is 0 or 1. */
    unsigned char bits[65];
    idx_t len = 0, steps = 0, i;
    idx_t t = n;

    /* Extract binary digits.  For n>0 this yields at least one bit. */
    while (t > 0) {
        bits[len++] = t & 1U;
        t >>= 1;
    }
    if (len == 0) {           /* n == 0 not expected, but keep safe */
        len = 1;
        bits[0] = 0;
    }

    /* Run‑length compression until the list has a single element. */
    while (len > 1) {
        idx_t new_bits[65];
        idx_t rlen = 0;        /* length of new_bits */

        for (i = 0; i < len; ) {
            idx_t val  = bits[i];
            idx_t cnt  = 1;
            while (i + cnt < len && bits[i + cnt] == val)
                ++cnt;
            new_bits[rlen++] = cnt;
            i += cnt;
        }

        /* Prepare next iteration. */
        for (i = 0; i < rlen; ++i)
            bits[i] = new_bits[i];
        len = rlen;
        ++steps;
    }

    /* By definition RUNS must be applied at least once. */
    if (steps == 0)
        steps = 1;

    return (seq_t)steps;
}
