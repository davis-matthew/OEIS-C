/*
 * A330729  Numbers k that are divisible by all integers less than or equal to
 *          the cube root of k.
 * https://oeis.org/A330729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k divisible by all integers ≤ ⌊k^{1/3}⌋
   OEIS A330729 – finite sequence; longest term is 420.
   Computation: for each m = ⌊k^{1/3}⌋ compute L = lcm(1,…,m),
   then list all multiples of L in the interval [m³,(m+1)³).
   The sequence ends when L exceeds the next interval.                                   */

#include <stdint.h>
#include <stdlib.h>

static seq_t seq_buffer[64];   /* space for all terms (sequence has ≤32 terms) */
static size_t seq_len = 0;

/* Euclidean gcd for seq_t (works for built‑in and arbitrary precision) */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Generate the whole sequence once (finite). */
static void init_sequence(void)
{
    seq_len = 0;                   /* reset */
    seq_t lcm = (seq_t)1;          /* lcm(1 … 0) = 1 */

    /* The sequence stops well before m = 20, but the loop is cheap. */
    for (idx_t m = 1; m <= 20; ++m)
    {
        seq_t m_seq = (seq_t)m;
        seq_t g = seq_gcd(lcm, m_seq);
        lcm = (lcm / g) * m_seq;          /* update lcm(1 … m) */

        /* interval bounds: [m³ , (m+1)³ – 1] */
        seq_t m_cubed = m_seq * m_seq * m_seq;
        seq_t next = m_seq + (seq_t)1;
        seq_t upper = next * next * next - (seq_t)1;

        /* first multiple of lcm not less than lower bound */
        seq_t first_mult = ((m_cubed + lcm - (seq_t)1) / lcm) * lcm;

        if (first_mult > upper)
            continue;                     /* no terms for this m */

        /* add all multiples of lcm inside the interval */
        for (seq_t k = first_mult; k <= upper; k += lcm)
        {
            seq_buffer[seq_len++] = k;
        }
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                   /* offset is 1, 0 is undefined */

    /* compute once on first call */
    static int initialized = 0;
    if (!initialized)
    {
        init_sequence();
        initialized = 1;
    }

    if (n > (idx_t)seq_len)
        return (seq_t)0;               /* out of range – undefined but safe */

    return seq_buffer[n - 1];
}
