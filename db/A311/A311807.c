/*
 * A311807  Coordination sequence Gal.3.12.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum_{k=1..n} inc(k) * where inc(1)=3, inc(2)=4, and
 *            for k≥3, * inc(k) = 5 if (k-3) mod 5 == 0, otherwise 4. *
 *            Equivalently, * for n ≥ 3: a(n) = 8 + 21*floor((n-2)/5) + * (if
 *            r≥1 then 5 else 0) + * (if r≥2 then 4 else 0) + * (if r≥3 then 4
 *            else 0) + * (if r≥4 then 4 else 0), * where r =...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 8;

    idx_t k = n - 2;          /* number of increments from 3 to n inclusive */
    idx_t cycles = k / 5;
    idx_t rem = k % 5;

    seq_t res = 8;                /* 1 + 3 + 4 = 8 for n≥2 */
    res += (seq_t)cycles * 21;    /* each full 5‑step cycle adds 21 */

    if (rem >= 1) res += 5;       /* inc(3) of the next cycle */
    if (rem >= 2) res += 4;       /* inc(4) */
    if (rem >= 3) res += 4;       /* inc(5) */
    if (rem >= 4) res += 4;       /* inc(6) */

    return res;
}
