/*
 * A390738  a(n) is the least possible difference between the areas of the
 *          largest and smallest trapezoids in a 3 element set of distinct
 *          integer-sided trapezoids whose base angles are 60 degrees that
 *          fill a regular hexagon of side n units.
 * https://oeis.org/A390738
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n*n - max_{1≤k≤n-1} min( k*(4*n-k), (n-k)*(3*n-k) )
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390738 – minimal defect for tiling a regular hexagon of side n
   with three distinct integer–sided trapezoids having base angles 60°.
   The formula derives from the family of triples */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2. For n < 2, behaviour is undefined;
       return 0 in this case. */
    if (n < 2)
        return 0;

    /* Constant 0.4188611699158103 = 2 - sqrt(10)/2 */
    const double KCONST = 0.4188611699158103;
    double dn = (double)n;
    double r = dn * KCONST;
    idx_t k1 = (idx_t)r;                     /* floor(r) – truncates towards zero */

    /* Evaluate k in the range [k1-1, k1+2] but bounded to [1, n-1] */
    idx_t lo = (k1 > 1 ? k1 - 1 : 1);
    idx_t hi = (k1 + 2 < n ? k1 + 2 : n - 1);

    seq_t max_minarea = 0;
    for (idx_t k = lo; k <= hi; ++k)
    {
        seq_t area2 = (seq_t)k * (seq_t)(4 * n - k);
        seq_t area3 = (seq_t)(n - k) * (seq_t)(3 * n - k);
        seq_t cur_min = (area2 < area3 ? area2 : area3);
        if (cur_min > max_minarea)
            max_minarea = cur_min;
    }

    seq_t total = (seq_t)3 * (seq_t)n * (seq_t)n;
    return total - max_minarea;
}
