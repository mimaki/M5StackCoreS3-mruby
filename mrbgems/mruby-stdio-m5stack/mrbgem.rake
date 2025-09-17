MRuby::Gem::Specification.new('mruby-stdio-m5stack') do |spec|
  spec.license = 'MIT'
  spec.author = 'Hiroshi Mimaki'
  spec.description = 'STDOUT._putc and STDIN._getc for M5Stack'

  spec.add_dependency('mruby-stdio')
end
