/*
 * A315331  Coordination sequence Gal.3.58.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1,6,10,16,22,26 for n < 6, * a(n) = a(r) + Δr + ((n-r)/6
 *            - 1) * Xr for n ≥ 6, * where r = n % 6, * Δr =
 *            {33,32,34,34,34,32} (for r = 0…5) * Xr = 34 if r is even, else
 *            32
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[6] = {
        (seq_t)1, (seq_t)6, (seq_t)10, (seq_t)16, (seq_t)22, (seq_t)26
    };
    if (n < 6) {
        return base[n];
    }

    idx_t r = n % 6;
    idx_t m = (n - r) / 6;          /* m ≥ 1 */
    seq_t result = base[r];

    /* special first increments Δr */
    seq_t delta;
    switch (r) {
        case 0: delta = (seq_t)33; break;
        case 1: delta = (seq_t)32; break;
        case 2: delta = (seq_t)34; break;
        case 3: delta = (seq_t)34; break;
        case 4: delta = (seq_t)34; break;
        case 5: delta = (seq_t)32; break;
        default: delta = (seq_t)0; break;   /* unreachable */
    }
    result += delta;

    if (m > 1) {
        seq_t x = (r % 2 == 0) ? (seq_t)34 : (seq_t)32;
        result += (seq_t)(m - 1) * x;
    }

    return result;
}
