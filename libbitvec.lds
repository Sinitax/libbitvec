LIBBITVEC_1.1 {
	global:
		libbitvec_errno;

		bitvec_init;
		bitvec_deinit;

		bitvec_alloc;
		bitvec_free;

		bitvec_reserve;
		bitvec_shrink;

		bitvec_get;
		bitvec_set;
		bitvec_clear;
		bitvec_setn;
	local: *;
};
