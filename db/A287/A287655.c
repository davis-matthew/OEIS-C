/*
 * A287655  Seven steps forward, six steps back.
 * https://oeis.org/A287655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A287655 – “Seven steps forward, six steps back”
    Recurrence:  a(0) = 0, a(1) = 1, …, a(13) = 1
                 a(n)   = a(n-1) + a(n-13) - a(n-14)   for n > 13           */

/* This implementation works for any seq_t, including arbitrary‑precision
   integer classes, because it uses only a fixed size array of 14 elements
   and no dynamic allocation.  The function is stateless apart from that
   small buffer, so it can be called repeatedly in any order.  */

/* Pre‑computed base values for indices 0..13. */
static const seq_t base_vals[14] = {
    0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1
};

/* A small ring buffer that holds the last 14 values needed for the
   recurrence.  The buffer is re‑initialised every call, which keeps
   the code simple and uses no dynamic memory. */
static seq_t buf[14];

/* The function itself. */
seq_t a(idx_t n)
{
    /* Initialisation of the buffer with the base values. */
    for (idx_t i = 0; i < 14; ++i)
        buf[i] = base_vals[i];

    /* For n ≤ 13 the answer already exists. */
    if (n < 14)
        return buf[n];

    /* Compute values for indices 14 .. n using the recurrence
       while only keeping the last 14 values in the ring buffer. */
    for (idx_t i = 14; i <= n; ++i)
    {
        /* Indices modulo 14 give the correct positions in the buffer. */
        idx_t idx_old   = (i - 1)  % 14;   /* a(i-1)  */
        idx_t idx_help  = (i - 13) % 14;   /* a(i-13) */
        idx_t idx_old2  = (i - 14) % 14;   /* a(i-14) */
        seq_t next = buf[idx_old] + buf[idx_help] - buf[idx_old2];
        buf[i % 14] = next;                /* store a(i) */
    }

    return buf[n % 14];
}
